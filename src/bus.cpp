#include "bus.hpp"
#include "kibitz_util.hpp"


namespace kibitz {

  bus::bus( void* zmq_context, const char* binding, int sock_type ) 
    :socket_(NULL) {
    socket_ = util::create_socket( zmq_context, sock_type );
    
  }

  bus::~bus() {
    util::close_socket( socket_ );

  }

  pub::pub( void* zmq_context, const char* binding )
    :bus( zmq_context, binding, ZMQ_PUB ) {
    util::check_zmq( zmq_bind( socket_, binding ) );
  }
  
  void pub::send( const string& json ) {
    DLOG(INFO) << "publishing " <<  json;
    util::send( socket_, json );
  } 

  sub::sub( void* zmq_context, const char* binding ) 
    :bus( zmq_context, binding, ZMQ_SUB ) {
    util::check_zmq( zmq_connect( socket_, binding ) );
    util::check_zmq( zmq_setsockopt( socket_, ZMQ_SUBSCRIBE, "", 0 ));
  }

}
