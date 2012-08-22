#ifndef worker_query_response_hpp
#define worker_query_response_hpp

#include "notification_message.hpp"
#include "worker_info.hpp"

namespace kibitz {

  


  class worker_query_response : public notification_message
  {
    worker_infos_t workers_;
  public :
    worker_query_response( ) ;
    worker_query_response( const ptree& json );
    ~worker_query_response();
    virtual string to_json() const ;
    const worker_infos_t& get_workers() const;
    const worker_infos_t& set_workers( const worker_infos_t& workers ) ;
    bool operator==( const worker_query_response& rval ) const; 
  };


  typedef shared_ptr< worker_query_response > worker_query_response_ptr_t;
}


#endif
