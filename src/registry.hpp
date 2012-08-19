#ifndef registry_hpp
#define registry_hpp

#include <zmq.h>
#include <map>
#include <set>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "heartbeat.hpp"
#include <sys/time.h>

using std::string;

/**
 *  This object aggregates heartbeats from workers and rebroadcasts
 *  discovery information to other workers.
 */
class registry {
  typedef boost::shared_ptr<kibitz::heartbeat> heartbeat_ptr_t;
  typedef std::map< int, heartbeat_ptr_t > worker_map_t;
  typedef std::map< string, worker_map_t > worker_type_map_t;


  void* publisher_socket_;
  void* inproc_pub_socket_;
  void* inproc_sub_socket_; 


  bool one_second_elapsed( timeval& last_send ); 
  
public:
  registry(  void* inproc_pub_socket, void* inproc_sub_socket, void* publisher_socket ); 
  virtual ~registry() ;
  void push_message( const string& msg ) ;
  void push_message( const kibitz::message& message );
  /**
   *  Thread function
   */
  void operator() () ;


};

#endif
