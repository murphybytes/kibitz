
#include <boost/test/minimal.hpp>
#include "common.hpp"
#include "heartbeat.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <vector>
#include <iostream>

#include "worker_query_response.hpp"
#include "worker_query.hpp"

namespace k = kibitz;
using boost::dynamic_pointer_cast;

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
  kibitz::message_ptr_t message = kibitz::message_factory( json );
  BOOST_CHECK( message != NULL );
  shared_ptr<kibitz::heartbeat> heartbeat_ptr = boost::dynamic_pointer_cast<kibitz::heartbeat>( message );
  BOOST_CHECK( heartbeat_ptr != NULL );

  std::vector< string > edges;
  string scalar;
  std::fstream stm( "../config/test-worker-b.yaml" );
  YAML::Parser parser( stm );
  YAML::Node doc ;
  parser.GetNextDocument( doc );
  YAML::Iterator it = doc.begin(); 
  it.first() >> scalar;
  it.second() >> edges;
  BOOST_CHECK( scalar == "in-edges" );
  BOOST_CHECK( edges.size() == 2 );
  ++it;
  BOOST_CHECK( it == doc.end() );
  
  k::worker_infos_t workers;
  k::worker_info worker;
  worker.worker_type = "test-worker-a";
  worker.worker_id = 1;
  worker.port = 9000;
  worker.host = "foo.com";
  workers.push_back(worker);
  k::worker_query_response response;
  response.set_workers( workers );
  json = response.to_json();
  std::cout << "worker query response " << json << std::endl;
  BOOST_CHECK( !json.empty() );
  k::worker_query_response_ptr_t worker_query_response_ptr = dynamic_pointer_cast< k::worker_query_response >( k::message_factory( json ) );
  BOOST_CHECK( worker_query_response_ptr != NULL );
  BOOST_CHECK( response == *worker_query_response_ptr );

  worker.worker_type = "test-worker-a";
  worker.worker_id = 2;
  worker.port = 9001;
  worker.host = "foo.com";
  workers.push_back(worker);
  response.set_workers( workers );
  std::cout << "with two workers " << response.to_json() << std::endl;
  worker_query_response_ptr = dynamic_pointer_cast< k::worker_query_response> ( k::message_factory( response.to_json() ) );
  BOOST_CHECK( worker_query_response_ptr != NULL );
  BOOST_CHECK( response == *worker_query_response_ptr );
  
  k::worker_query wq( "worker-type-a" );
  json = wq.to_json();
  std::cout << "worker query " << json << std::endl;
  BOOST_CHECK( !json.empty() );
  k::worker_query_ptr_t worker_query_ptr = dynamic_pointer_cast<k::worker_query>( k::message_factory( json ) );
  BOOST_CHECK( worker_query_ptr != NULL );
  BOOST_CHECK( worker_query_ptr->worker_type() == "worker-type-a" );

  return 0;
}
