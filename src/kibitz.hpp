#ifndef __KIBITZ_HPP__
#define __KIBITZ_HPP__

#include "common.hpp"
#include "collaboration_message.hpp"
#include "notification_message.hpp"


namespace kibitz {
  typedef std::vector< collaboration_message_ptr_t > collaboration_messages_t;
  typedef void ( *callback )( const string& job_id, const collaboration_messages_t& messages  );

  void initialize( int argc, char* argv[] ) ; 
  void start() ;
  void terminate();
  void set_message_handler( callback fn );
  void send_message( const string& job_id, const collaboration_message& message ) ;
  void send_notification( const notification_message& message ) ;
  
  

}

#endif
