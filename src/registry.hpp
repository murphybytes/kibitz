#ifndef registry_hpp
#define registry_hpp

#include <zmq.h>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "heartbeat.hpp"
#include <sys/time.h>

using std::string;
using std::multimap;
using std::pair;

class registry {

  typedef multimap< string, kibitz::heartbeat > map_t;


  map_t map_;
  void* publisher_socket_;
  void* inproc_pub_socket_;
  void* inproc_sub_socket_; 


  bool one_second_elapsed( timeval& last_send ); 
public:
  registry(  void* inproc_pub_socket, void* inproc_sub_socket, void* publisher_socket ); 
  virtual ~registry() ;
  void push_message( const string& msg ) ;
  void push_message( const kibitz::message& message );
  void operator() () ;


};

#endif
