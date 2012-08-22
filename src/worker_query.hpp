#ifndef worker_query_hpp
#define worker_query_hpp

#include "notification_message.hpp"

namespace kibitz {
  class worker_query : public notification_message {
    const string worker_type_;
  public:
    worker_query( const string& worker_type ) ;
    worker_query( const ptree& tree ) ;
    virtual ~worker_query();
    const string& worker_type() const { return worker_type_; }
    
    virtual string to_json() const;
  };

  typedef shared_ptr<worker_query> worker_query_ptr_t;
}

#endif
