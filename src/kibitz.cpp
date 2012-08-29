#include "kibitz.hpp"
#include "context.hpp"
//#include "heartbeat_sender.hpp"
#include <signal.h>


namespace kibitz {
  context* context_ =  NULL;





  

  void validate_command_line( const po::variables_map& command_line ) {

    if( !command_line.count( "worker-id" ) ) {
      throw runtime_error( string("Missing required worker-id argument") );
    }

    if( !command_line.count( "worker-type" ) ) {
      throw runtime_error(  string("Missing required worker-type argument") );
    }



  } 

  void initialize( int argc, char* argv[] ) {
    assert( !context_ );

    InitGoogleLogging( argv[0] );
    InstallFailureSignalHandler();
    DLOG(INFO) << "initialize start";
    fs::path path( argv[0] );
    stringstream ss ;
    ss << path.filename() << " options" ;
    po::options_description options(ss.str());
    options.add_options() 
      ( "help,h", "Show help message" )
      ( "worker-id,I", po::value< int >(), "(Required) Integer that identifies worker. Must be unique with worker type" )
      ( "worker-type,T", po::value< string >(), "(Required) Name of the type of worker." )
      ( "configuration-file,f", po::value< string >(), "(Required) Path to configuration file containing in edges and out edges" ) 
      ( "heartbeat-binding,b", po::value<string>(), "zmq tcp binding for publishing heartbeats" )
      ( "discovery-binding,d", po::value<string>(), "zmq tcp binding for recieving locator information" )
      ( "publish-port,p", po::value<int>()->default_value(8999), "Port to publish for collaboration messages" )
      ( "notification-port,P", po::value<int>()->default_value(9999), "Port to publish notification messages")
      ( "context-threads,t", po::value< int >()->default_value( 1 ), "Thread count passed to zmq_init" )
      ( "heartbeat-frequency,h", po::value< int >()->default_value( 1000 ), "Heartbeat frequency in milliseconds" )
      ;
    po::variables_map command_line;
    po::store( po::parse_command_line( argc, argv, options ), command_line );
    po::notify( command_line );


   if( command_line.count( "help" ) ) {
      std::cout << options << std::endl;
      exit( 1 );
    }
    

    validate_command_line( command_line );
 
    context_=  new context( command_line ) ;

  }

  void start() {
    assert( context_ );
    context_->start();
  }

  void terminate() {
    if( context_ ) {
      context_->terminate();
    }
  }

  void set_in_message_handler( collaboration_callback fn ) {
    context_->register_inedge_message_handler( fn );
  }

  void set_initialization_notification_handler( initialization_callback fn ) {
    context_->register_initialization_notification_handler( fn ) ;
  }
  
  

  void send_out_message( const string& payload ) {
    context_->send_out_message( payload );
  }

  void send_notification_message( const string& payload ) {
    context_->send_notification_message( payload );
  }
  

}

