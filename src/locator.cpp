#include <assert.h>
#include <zmq.h>
#include "locator.hpp"
#include <glog/logging.h>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include "registry.hpp"
#include <signal.h>
namespace fs = boost::filesystem;
using boost::format;
using namespace google;
using std::string;
namespace po = boost::program_options;

using kibitz::util::create_socket;
using kibitz::util::check_zmq;
using kibitz::util::close_socket;

string pid_file;
void signal_callback( int sig ) {
  fs::path path( pid_file );
  if( fs::exists( path ) ) {
    fs::remove_all( path );
  }
}

int main( int argc, char* argv[] ) {

  po::options_description options( "locator" );
  options.add_options()
    ( "help,h", "Show help message" )
    ("heartbeat-binding,b", po::value<string>()->default_value("tcp://*:5556"), "Binding to listen for worker heartbeats" )
    ("port,p", po::value<int>()->default_value(5557), "Port used to distribute locator messages" )
    ("context-threads,t", po::value<int>()->default_value(1), "zmq context thread count")
    ("daemon,d", "Run as a daemon" )
    ( "pid-file", po::value<string>()->default_value( "/var/run/kibitz-locator.pid" ), "Location of pid file for daemon mode" )
    ;

  po::variables_map command_line;
  po::store( po::parse_command_line( argc, argv, options ), command_line );
  po::notify( command_line );

  if( command_line.count("daemon") ) {
    pid_file = command_line["pid-file"].as<string>();
    kibitz::util::daemonize( pid_file );
    signal( SIGINT, signal_callback );
    
  }



  InitGoogleLogging( argv[0] );
  InstallFailureSignalHandler( );
  DLOG(INFO) << "Start locator" ;


  const char* in_binding = command_line["heartbeat-binding"].as<string>().c_str() ;
  const int port = command_line["port"].as<int>() ;
  int exit_code = 0;
  int rc = 0;

  if( command_line.count("help") ) {
    std::cout << options << std::endl;
    exit( 1 );
  }

  void* context = zmq_init( command_line["context-threads"].as<int>() );
  void* inproc_pub_socket = NULL;
  void* inproc_sub_socket = NULL;
  void* outsocket = NULL;
  void* insocket = NULL;

  if( !context ) {
    LOG(ERROR) << "Could not create zmq context " << zmq_errno();
    return 1;
  }

  try {

    insocket = create_socket(  context, ZMQ_PULL );
    check_zmq( zmq_bind( insocket, in_binding ) );
    inproc_pub_socket = create_socket(  context, ZMQ_PUB );
    check_zmq( zmq_bind( inproc_pub_socket, "inproc://x" ) );
    inproc_sub_socket = create_socket( context, ZMQ_SUB );
    check_zmq( zmq_connect( inproc_sub_socket, "inproc://x" ) );
    check_zmq( zmq_setsockopt( inproc_sub_socket, ZMQ_SUBSCRIBE, "", 0 ) );
    outsocket = create_socket( context, ZMQ_PUB );
    string pub_binding = (format( "tcp://*:%1%" ) % command_line["port"].as<int>()).str();
    LOG(INFO) << "Locator will publish on " << pub_binding;
    check_zmq( zmq_bind( outsocket, pub_binding.c_str() ) );

    registry reg(  inproc_pub_socket, inproc_sub_socket, outsocket );
    boost::thread sender_thread( reg );

    try {
      while( true ) {
	string message;
	kibitz::util::recv( insocket, message );
	if( !message.empty() ) {
	  DLOG(INFO) << "got message -> " << message ;
	  reg.push_message(  message  );
	}
      }
    } catch( const kibitz::util::queue_interrupt& ) {
      LOG(INFO) << "Caught signal shutting down" ;
    } catch( const std::exception& ex ) {
      LOG(ERROR) << "Something bad killed us. What => " << ex.what() ;
      exit_code = 1;
    }

    kibitz::inproc_notification_message inproc_notification_message( kibitz::message::stop );
    reg.push_message( inproc_notification_message );
    sender_thread.join();


  } catch( const std::exception& ex ) {
    exit_code = 1;
    LOG(ERROR) << "An exception killed worker locator " << ex.what() ;
  }

  close_socket( inproc_pub_socket );
  close_socket( inproc_sub_socket );
  close_socket( outsocket );
  close_socket( insocket ) ;
  zmq_term( context );
  


  return exit_code;

}


