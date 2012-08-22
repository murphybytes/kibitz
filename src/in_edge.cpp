#include "in_edge.hpp"
#include "kibitz_util.hpp"
#include "worker_map.hpp"



namespace kibitz {

  in_edge::in_edge( context* context, const string& worker_id ) 
    :context_( context ),
     worker_id_( worker_id ) {
  }

  in_edge::~in_edge() {
  }

  void in_edge::operator()() {
    //LOG(



  }

}
