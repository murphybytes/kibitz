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

int recv( void* socket, string& message, int flag) {
  assert( message.empty() );
  const int buffer_size = 2048;
  char buffer[buffer_size];
  int received = zmq_recv( socket, buffer, buffer_size, flag );
  if( received == -1 ) {
    int err = zmq_errno() ;
    if( err == EAGAIN ) {
      LOG(INFO) << "Waiting for messages";
      sleep( 1 );
    }
  } else {
    message.append( buffer, received ) ;
  }
  return received;
}




void send( void* socket, const string& message ) {
  int sent = zmq_send( socket, (void*)message.data(), message.length(), 0 );
  if( sent == -1 ) {
    LOG(ERROR) << "Could not send message. Err " << zmq_errno() << " Message " << message ;
  }
}

int main( int argc, char* argv[] ) {

  InitGoogleLogging( argv[0] );
  InstallFailureSignalHandler( );
  DLOG(INFO) << "Start locator" ;

  po::options_description options( "locator" );
  options.add_options()
    ( "help,h", "Show help message" )
    ("heartbeat-binding,b", po::value<string>(), "Multicast binding to receive heartbeats" )
    ("port,p", po::value<int>(), "Port used to distribute locator messages" )
    ("context-threads,t", po::value<int>()->default_value(1), "zmq context thread count")
    ;

  po::variables_map command_line;
  po::store( po::parse_command_line( argc, argv, options ), command_line );
  po::notify( command_line );

  const char* in_binding = command_line["heartbeat-binding"].as<string>().c_str() ;
  const char* out_binding = command_line["discovery-binding"].as<string>().c_str() ;

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



    
    /*
    void* outsocket = zmq_socket( context, ZMQ_PUB ) ;

    if( !outsocket ) {
      LOG(ERROR) << "Could not create outbound socket. Error " << zmq_errno() ;
      exit( 1 );
    }

    rc = zmq_connect( outsocket, out_binding );

    if( rc ) {
      LOG(ERROR) << "Could not connect broadcast socket " << out_binding << " " << zmq_errno() ;
      exit( 1 ); 
    }

    */

    //std::multimap<string, heartbeat > heartbeats;
    registry reg( context, command_line["port"].as<int>() );

    while( true ) {
      string message;
      recv( insocket, message, ZMQ_DONTWAIT );
      if( !message.empty() ) {
	DLOG(INFO) << "got message -> " << message ;
	//	send( outsocket, message );
	reg.push_message(  message  );
	
      }
      
    }

    zmq_close( insocket ) ;
    //zmq_close( outsocket ) ;




    zmq_term( context );
  }

  

  return 0;

}


