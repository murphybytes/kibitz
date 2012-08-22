#ifndef in_edge_hpp
#define in_edge_hpp

#include "context.hpp"

namespace kibitz {
  class in_edge {
    context* context_;
    const string worker_id_;
  public:
    in_edge( context* context, const string& worker_id ) ;
    virtual ~in_edge() ;

    void operator()() ;
  };
}

#endif
