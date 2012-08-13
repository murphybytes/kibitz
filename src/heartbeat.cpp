#include "heartbeat.hpp"

namespace kibitz {

  heartbeat::heartbeat( context* ctx ) 
    :worker_type_( ctx->get_config()["worker-type"].as<string>()  ),
     worker_id_( ctx->get_config()["worker-id"].as<int>() ),
     host_name_( "xxxx" ),
     pid_(getpid()),
     port_(ctx->get_config()["tcp-port"].as<int>()) {


  }

  heartbeat::~heartbeat() {
  }


  string heartbeat::to_json() const {

    clock_t ticks = clock();
    stringstream stm;
    ptree tree;
    tree.put( "worker_type", worker_type_ );
    tree.put( "worker_id", worker_id_ );
    tree.put( "host", host_name_ );
    tree.put( "process_id", pid_ );
    tree.put( "port" , port_ );
    tree.put( "ticks", ticks );
    pt::json_parser::write_json( stm, tree );
    return stm.str();
  }



}
