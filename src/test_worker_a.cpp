#include "kibitz.hpp"


void message_handler( const string& job_id, const kibitz::collaboration_messages_t& messages ) ; 


int main( int argc, char* argv[] ) {
  std::cout << "starting" << std::endl;
  int result = 0;

  try {
    kibitz::initialize( argc, argv );
    kibitz::set_message_handler( message_handler );
    kibitz::start();
    kibitz::terminate();
  } catch( std::exception& e ) {
    result = 1;
    std::cout << "Program failed. " << e.what() << std::endl;
  }
  
  return result;


}


void message_handler( const string& job_id, const kibitz::collaboration_messages_t& messages )  {
  DLOG(INFO) << "messages for " << job_id << " got " << messages.size() ;
} 
