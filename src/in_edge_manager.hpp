#ifndef in_edge_manager_hpp
#define in_edge_manager_hpp

#include "kibitz.hpp"
#include "worker_info.hpp"

namespace kibitz {
  class in_edge_manager {

    struct collaboration_context_t {
      int count_items;
      std::map< string, collaboration_messages_t > job_messages;
      zmq_pollitem_t* pollitems;
    };

    context& context_;
    void create_bindings( zmq_pollitem_t** pollitems, int& count_items, int& size_items );
    void create_binding( const worker_info& info, zmq_pollitem_t& pollitem );
    void release_bindings( zmq_pollitem_t* pollitems, int count_items ) ;
    void handle_notification_message( zmq_pollitem_t** pollitems, int& count_items, int& size_items );
    void handle_collaboration_message( collaboration_context_t& context );
  public:
    in_edge_manager( context& ctx );
    virtual ~in_edge_manager() ;
    void operator()() ;

  };
}

#endif
