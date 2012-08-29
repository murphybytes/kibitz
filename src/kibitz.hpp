#ifndef __KIBITZ_HPP__
#define __KIBITZ_HPP__

#include "common.hpp"
#include "collaboration_message.hpp"
#include "notification_message.hpp"


namespace kibitz {
  typedef std::vector< string > collaboration_messages_t;
  typedef void ( *collaboration_callback )( const collaboration_messages_t& messages  );
  typedef void ( *initialization_callback )( void );

  void initialize( int argc, char* argv[] ) ; 
  void start() ;
  void terminate();
  void set_in_message_handler( collaboration_callback fn );
  void set_initialization_notification_handler( initialization_callback fn );
  void send_out_message( const string& payload ) ;
  void send_notification_message( const string& payload ) ;
  
  

}

#endif
