#include "common.hpp"


namespace kibitz {

  void recv( void* socket, string& message ) {
    message.clear();
    zmq_msg_t zmq_msg;
    zmq_msg_init( &zmq_msg );
    zmq_recvmsg( socket, &zmq_msg,  0 );
    int size = zmq_msg_size( &zmq_msg );
    message.append( (char*)zmq_msg_data(&zmq_msg), size );
    zmq_msg_close( &zmq_msg );
  }

  void send( void* socket, const string& message ) {
    zmq_msg_t zmq_msg;
    zmq_msg_init_size( &zmq_msg, message.length() );
    memcpy( zmq_msg_data( &zmq_msg ), message.data(), message.length() );
    zmq_sendmsg( socket, &zmq_msg, 0 );
    zmq_msg_close( &zmq_msg );
  }



}
