#ifndef in_edge_manager_hpp
#define in_edge_manager_hpp

#include "kibitz.hpp"
#include "worker_info.hpp"
#include "notification_message.hpp"

namespace kibitz {
  class in_edge_manager {

    struct collaboration_context_t {
      int count_items;
      std::map< string, std::map< string, string >  > job_messages;
      zmq_pollitem_t* pollitems;
      std::vector<string> get_job_messages( const string& job_id ) {
	std::vector<string> result;
	for( std::map<string,string>::iterator it = job_messages[job_id].begin(); it != job_messages[job_id].end(); ++it ){
	  result.push_back( it->second );
	}
	return result;
      }
    };

    context& context_;
    const string worker_type_ ;
    const int worker_id_;
    void create_bindings( zmq_pollitem_t** pollitems, int& count_items, int& size_items );
    void create_binding( const worker_info& info, zmq_pollitem_t& pollitem );
    void release_bindings( zmq_pollitem_t* pollitems, int count_items ) ;
    void handle_notification_message( zmq_pollitem_t** pollitems, int& count_items, int& size_items );
    void handle_collaboration_message( collaboration_context_t& context );
    bool all_messages_arrived( const string& job_id, collaboration_context_t& collab_context ) const;
    void check_and_start_job( notification_message_ptr_t message ) ;
  public:
    in_edge_manager( context& ctx );
    virtual ~in_edge_manager() ;
    void operator()() ;

  };
}

#endif
