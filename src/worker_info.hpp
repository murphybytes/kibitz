#ifndef worker_info_hpp
#define worker_info_hpp

#include "notification_message.hpp"


namespace kibitz {
  struct worker_info {
    string worker_type;
    string host;
    int worker_id ;
    int port;
  };

  typedef std::vector< worker_info > worker_infos_t;

}





#endif
