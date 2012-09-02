/********************************************************************
 *  Program to run kibitz regression tests.  We use a number of 
 *  environment variables to control tests
 * 
 *  ENVIRONMENT VARIABLES
 *   
 *  KIBITZ_ROLE = CHILD | ROOT 
 *  If valued as CHILD the program will handle inbound collaboration messages
 *  If ROOT, we handle the initialization notification to start the 
 *  test running.  
 ********************************************************************/

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

using boost::lexical_cast;
using namespace boost::filesystem;

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <boost/foreach.hpp>
#include "kibitz.hpp"
#include <assert.h>

using namespace std;
using kibitz::context_information_t;

void message_handler( const kibitz::collaboration_messages_t& messages ) ; 
void notification_handler( );
void record_in_message( const string& in );
void record_out_message( const string& out );
void record_message( const string& filename, const string& message );


int main( int argc, char* argv[] ) {
  std::cout << "starting" << std::endl;
  int result = 0;

  try {
    kibitz::initialize( argc, argv );
    // ROOT, CHILD
    string role = getenv( "KIBITZ_ROLE" );
    assert( !role.empty() );

    if( role == "ROOT" ) {
      kibitz::set_initialization_notification_handler( notification_handler );
    } else if( role == "CHILD" ) {
      kibitz::set_in_message_handler( message_handler );      
    }

    kibitz::start();
    kibitz::terminate();
  } catch( std::exception& e ) {
    result = 1;
    std::cout << "Program failed. " << e.what() << std::endl;
  }
  
  return result;


}


void message_handler( const kibitz::collaboration_messages_t& messages )  {
  
  stringstream stm;
  BOOST_FOREACH( const string& message, messages ) {
    stm << message << "\n";
  }

  record_in_message( stm.str() );
  
  string payload = lexical_cast<string>( boost::uuids::random_generator()());
  kibitz::send_out_message( payload );
} 


void notification_handler( ) {
  string payload = lexical_cast<string>( boost::uuids::random_generator()());
  record_out_message( payload );
  kibitz::send_out_message( "initial message" );
}




void record_message( const string& filename, const string& message ) {

  path test_file_path = (current_path() /= "test") /= filename;
  ofstream stm ;
  stm.open( test_file_path.c_str() );
  stm << message;
  stm.close();

}

void record_out_message( const string& out ) {
  context_information_t context_info;
  kibitz::get_context_information( context_info );
  stringstream stm;
  stm << context_info.worker_type << "." << context_info.worker_id << ".out";
  record_message( stm.str(), out );

}

void record_in_message( const string& in ) {
  context_information_t context_info;
  kibitz::get_context_information( context_info );
  stringstream stm;
  stm << context_info.worker_type << "." << context_info.worker_id << ".in";
  record_message( stm.str(), in );
}
