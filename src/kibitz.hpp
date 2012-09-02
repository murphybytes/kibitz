#ifndef __KIBITZ_HPP__
#define __KIBITZ_HPP__
/*
#include "common.hpp"
#include "collaboration_message.hpp"
#include "notification_message.hpp"
*/

#include <string>
#include <vector>

namespace kibitz {
  typedef std::string payload_t;
  typedef std::vector< payload_t > collaboration_messages_t;
  typedef void ( *collaboration_callback )( const collaboration_messages_t& messages  );
  typedef void ( *initialization_callback )( void );

  struct context_information_t {
    std::string worker_type;
    std::string job_id;
    int worker_id;

  };

  void initialize( int argc, char* argv[] ) ; 
  void start() ;
  void terminate();
  void set_in_message_handler( collaboration_callback fn );
  void set_initialization_notification_handler( initialization_callback fn );
  void send_out_message( const payload_t& payload ) ;
  void send_notification_message( const payload_t& payload ) ;
  /**
   * Retrieves information about worker, for diagnostic purposes only
   */
  void get_context_information( context_information_t& context_information );
  

}

#endif
