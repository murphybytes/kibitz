#ifndef heartbeat_hpp
#define heartbeat_hpp

#include "heartbeat.hpp"


namespace kibitz {



  heartbeat::heartbeat( const boost::program_options::variables_map& config  ) 
    :notification_message( "heartbeat" ),
     worker_type_( config["worker-type"].as<string>()  ),
     worker_id_( config["worker-id"].as<int>() ),
     host_name_( "xxxx" ),
     pid_(getpid()),
     port_( config["tcp-port"].as<int>()),
     ticks_(0) {


  }

  heartbeat::heartbeat(  const ptree& json ) 
    :notification_message( "heartbeat" ),
     worker_type_( json.get<string>( "worker_type" ) ),
     worker_id_( json.get<int>( "worker_id" ) ),
     host_name_( json.get<string>( "host" )),
     pid_( json.get<int>("process_id")),
     port_( json.get<int>("port")),
     ticks_(json.get<int>("ticks")) {

  }

  heartbeat::~heartbeat() {
  }

  void heartbeat::increment_tick_count() {
    static int counter = 0;
    ticks_ = ++counter;
  }


  string heartbeat::to_json() const {

    stringstream stm;
    ptree tree;
    tree.put( "message_type", "notification" );
    tree.put( "notification_type", "heartbeat" );
    tree.put( "worker_type", worker_type_ );
    tree.put( "worker_id", worker_id_ );
    tree.put( "host", host_name_ );
    tree.put( "process_id", pid_ );
    tree.put( "port" , port_ );
    tree.put( "ticks", ticks_ );
    boost::property_tree::json_parser::write_json( stm, tree );
    return stm.str();
  }

  bool heartbeat::operator<(const heartbeat& comp ) const {
    if( worker_type_ < comp.worker_type_ ) {
      return worker_id_ < comp.worker_id_ ;
    }

    return false;
  }

  const string& heartbeat::worker_type() const {
    return worker_type_;
  }

  const int& heartbeat::worker_id() const {
    return worker_id_;
  }



}


#endif
