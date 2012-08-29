#include "kibitz.hpp"


void message_handler( const kibitz::collaboration_messages_t& messages ) ; 
void notification_handler( );

int main( int argc, char* argv[] ) {
  std::cout << "starting" << std::endl;
  int result = 0;

  try {
    kibitz::initialize( argc, argv );
    kibitz::set_in_message_handler( message_handler );
    kibitz::set_initialization_notification_handler( notification_handler );
    kibitz::start();
    kibitz::terminate();
  } catch( std::exception& e ) {
    result = 1;
    std::cout << "Program failed. " << e.what() << std::endl;
  }
  
  return result;


}


void message_handler( const kibitz::collaboration_messages_t& messages )  {
  DLOG(INFO) << "message_handler got "  << messages.size()  << " messages";
  string payload ;
  BOOST_FOREACH( const string& message, messages ) {
    if( payload.empty() ) {
      payload = message;
    } else {
      payload += " > ";
      payload += message;
    }
    
  }

  payload += " > ";
  payload += "next message";

  kibitz::send_out_message( payload );
} 


void notification_handler( ) {
  DLOG(INFO) << ">>>>>>>>>>>>>>> NOTIFY <<<<<<<<<<<<<<<<<<" ;
  kibitz::send_out_message( "initial message" );
}
