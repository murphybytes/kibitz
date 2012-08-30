#include "job_initialization_message.hpp"


namespace kn = kibitz::notification ;

namespace kibitz {

  job_initialization_message::job_initialization_message( const ptree& json )
    :notification_message( kn::JOB_INITIALIZATION ),
     worker_type_( json.get<string>(kn::WORKER_TYPE) ),
     worker_id_( json.get<int>(kn::WORKER_ID) ) {
  }

  job_initialization_message::job_initialization_message( const string& worker_type, int worker_id ) 
    :notification_message( kn::JOB_INITIALIZATION ), 
      worker_type_(worker_type),
      worker_id_(worker_id) {
      
  }

  
  string job_initialization_message::to_json() const {
    stringstream stm;
    ptree tree;
    notification_message::populate_header( tree );
    tree.put( kn::WORKER_TYPE, worker_type_ );
    tree.put( kn::WORKER_ID, worker_id_ );
    boost::property_tree::json_parser::write_json( stm, tree );
    return stm.str();
  }

}
