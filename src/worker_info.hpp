#ifndef worker_info_hpp
#define worker_info_hpp

#include "notification_message.hpp"


namespace kibitz {
  struct worker_info {
    string worker_type;
    string host;
    int worker_id ;
    int port;
    bool operator==( const worker_info& rval ) {
      if( worker_type == rval.worker_type ) 
	if( host == rval.host )
	  if( worker_id == rval.worker_id )
	    if( port == rval.port )
	      return true;

      return false;

    }
    bool operator!=( const worker_info& rval ) {
      return !operator==( rval );
    }
  };

  typedef std::vector< worker_info > worker_infos_t;



}





#endif
