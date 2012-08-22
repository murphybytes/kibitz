#include "worker_query_response.hpp"
#include <boost/foreach.hpp>

namespace kibitz {

    worker_query_response::worker_query_response( )
      :notification_message( "worker_query_response" ) {
    }

    worker_query_response::worker_query_response( const ptree& json )
      :notification_message( "worker_query_response" ) {
      
      BOOST_FOREACH( const ptree::value_type& worker, json.get_child( "workers" ) ) {
	worker_info info;
	info.worker_type = worker.second.get<string>("worker_type");	
	info.host = worker.second.get<string>("host");
	info.worker_id = worker.second.get<int>("worker_id");
	info.port = worker.second.get<int>("port");
	
	workers_.push_back( info );
      }
    }

  worker_query_response::~worker_query_response( ) {
  }


  bool worker_query_response::operator==( const worker_query_response& rval ) const {
    worker_infos_t::const_iterator it = rval.workers_.begin();
    BOOST_FOREACH( worker_info info, workers_ ) {
      if( it == rval.workers_.end() ) {
	return false;
      }
      
      if( info != *it ) {
	return false;
      }

      ++it;

    }
    return true;
  }

  string worker_query_response::to_json() const {
    stringstream stm;
    ptree tree;
    notification_message::populate_header( tree );
    ptree array;
    BOOST_FOREACH( worker_info wi, workers_ ) {
      ptree worker;
      worker.put( "worker_type", wi.worker_type );
      worker.put( "host", wi.host );
      worker.put( "worker_id", wi.worker_id );
      worker.put( "port", wi.port );
      array.push_back( std::make_pair( "", worker ) );
    }
    
    tree.push_back( std::make_pair( "workers", array ) ); 
    boost::property_tree::json_parser::write_json( stm, tree );
    return stm.str();
  }


  const worker_infos_t& worker_query_response::set_workers( const worker_infos_t& workers ) {
    workers_ = workers;
    return workers_;
  }

  const worker_infos_t& worker_query_response::get_workers() const {
    return workers_;
  }


} 
