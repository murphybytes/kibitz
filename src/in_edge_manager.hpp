#ifndef in_edge_manager_hpp
#define in_edge_manager_hpp

#include "kibitz.hpp"
#include "worker_info.hpp"

namespace kibitz {
  class in_edge_manager {

    context& context_;
    void create_bindings( zmq_pollitem_t** pollitems, int& count_items, int& size_items );
    void create_binding( const worker_info& info, zmq_pollitem_t& pollitem );
    void release_bindings( zmq_pollitem_t* pollitems, int count_items ) ;
  public:
    in_edge_manager( context& ctx );
    virtual ~in_edge_manager() ;
    void operator()() ;

  };
}

#endif
