#include "worker_query_response.hpp"


namespace kibitz {

    worker_query_response::worker_query_response( )
      :notification_message( "worker_query_response" ){
    }

    worker_query_response::worker_query_response( const ptree& json )
      :notification_message( "worker_query_response" ) {
    }

  string worker_query_response::to_json() const {
    string response;
    return response;
  }

  worker_infos_t worker_query_response::get_workers() {
    worker_infos_t result;
    return result;
  }


} 
