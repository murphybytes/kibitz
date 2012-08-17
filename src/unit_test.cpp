#include <boost/test/minimal.hpp>
#include "common.hpp"
#include "heartbeat.hpp"


int test_main( int argc, char* argv[] ) {
  
  po::options_description options("test");
  options.add_options() 
    ( "help,h", "Show help message" )
    ( "worker-id,I", po::value< int >()->default_value(1), "xxx" )
    ( "worker-type,T", po::value< string >()->default_value("test-worker"), "(Required) Name of the type of worker." )
    ( "multicast-binding,b", po::value<string>()->default_value("xxx"), "(Required) zmq epgm multicast binding" )
    ( "tcp-port,p", po::value<int>()->default_value(8999), "Port for tcp based zmq messages" )
    ( "context-threads,t", po::value< int >()->default_value( 1 ), "Thread count passed to zmq_init" )
    ;
  po::variables_map command_line;
  po::store( po::parse_command_line( argc, argv, options ), command_line );
  po::notify( command_line );

  kibitz::heartbeat heartbeat( command_line );
  string json = heartbeat.to_json() ;
  BOOST_CHECK( !json.empty() );
  kibitz::message_ptr message = kibitz::message_factory( json );
  BOOST_CHECK( message != NULL );
  shared_ptr<kibitz::heartbeat> heartbeat_ptr = boost::dynamic_pointer_cast<kibitz::heartbeat>( message );
  BOOST_CHECK( heartbeat_ptr != NULL );
  return 0;
}
