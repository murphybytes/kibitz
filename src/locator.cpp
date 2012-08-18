#include <assert.h>
#include <zmq.h>
#include "locator.hpp"
#include <glog/logging.h>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include "registry.hpp"


using namespace google;
using std::string;
namespace po = boost::program_options;


int main( int argc, char* argv[] ) {

  InitGoogleLogging( argv[0] );
  InstallFailureSignalHandler( );
  DLOG(INFO) << "Start locator" ;

  po::options_description options( "locator" );
  options.add_options()
    ( "help,h", "Show help message" )
    ("heartbeat-binding,b", po::value<string>()->default_value("tcp://*:5556"), "Multicast binding to receive heartbeats" )
    ("port,p", po::value<int>()->default_value(5557), "Port used to distribute locator messages" )
    ("context-threads,t", po::value<int>()->default_value(1), "zmq context thread count")
    ;

  po::variables_map command_line;
  po::store( po::parse_command_line( argc, argv, options ), command_line );
  po::notify( command_line );

  const char* in_binding = command_line["heartbeat-binding"].as<string>().c_str() ;
  const int port = command_line["port"].as<int>() ;
  int exit_code = 0;
  int rc = 0;

  if( command_line.count("help") ) {
    std::cout << options << std::endl;
    exit( 1 );
  }

  void* context = zmq_init( command_line["context-threads"].as<int>() );
  
  if( context ) {
    void* insocket = zmq_socket( context, ZMQ_PULL );
    
    if( !insocket ) {
      LOG(ERROR) << "Could not create inbound socket. Error  " << zmq_errno();
      exit( 1 );
    }

    rc = zmq_bind( insocket, in_binding );

    if( rc ) {
      LOG(ERROR) << "Could not connect to heartbeat broadcast " << in_binding << " " << zmq_errno() ;
      exit( 1 );
    }


    registry reg( context, command_line["port"].as<int>() );
    boost::thread sender_thread( reg );

    try {
      while( true ) {
	string message;
	kibitz::util::recv( insocket, message );
	if( !message.empty() ) {
	  DLOG(INFO) << "got message -> " << message ;
	  //	send( outsocket, message );
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

    zmq_close( insocket ) ;
    zmq_term( context );
  }

  

  return exit_code;

}


