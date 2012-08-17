#include "registry.hpp"
#include <zmq.h>
#include "locator.hpp"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using boost::property_tree::ptree;

using namespace google;

registry::registry( void* context, int port ) 
  :map_ptr_( new map_t() ),
   context_( context ),
   port_(port) {
}

registry::~registry() {
}


void registry::push_message( const string& msg ) {
  boost::mutex::scoped_lock( guard_ );
  stringstream stm;
  stm << msg;
  ptree tree;
  boost::property_tree::json_parser::read_json( stm, tree );  
  kibitz::heartbeat hb( tree );
  map_ptr_->insert( pair<string, kibitz::heartbeat>( hb.key(), hb ) );
}

void registry::operator()() {
  DLOG(INFO) << "starting registry thread";
  stringstream stm;
  stm << "tcp://*:" << port_;
  void* socket = zmq_socket( context_, ZMQ_REP );
  if( !socket ) {
    LOG(ERROR) << "Socket creation failed " << zmq_errno() ;
    return;
  }

  int rc = zmq_bind( socket, stm.str().c_str() );

  if( rc ) {
    LOG(ERROR) << "Could not bind to " << stm.str() << " Error " << zmq_errno() ;
    return;
  }

  while( true ) {
    string message;
    int status = recv( socket, message ) ;
    if( status < 0 ) {
      break;
    }
    // TODO: 
    
  }
}
