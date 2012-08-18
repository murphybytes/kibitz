#ifndef registry_hpp
#define registry_hpp

#include <zmq.h>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "heartbeat.hpp"

using std::string;
using std::multimap;
using std::pair;

class registry {

  typedef multimap< string, kibitz::heartbeat > map_t;
  typedef shared_ptr< map_t > map_ptr_t;

  map_ptr_t map_ptr_;
  void* context_;
  void* inproc_socket_;
  int port_;
public:
  registry( void* context, int port ); 
  virtual ~registry() ;
  void push_message( const string& msg ) ;
  void push_message( const kibitz::message& message );
  void operator() () ;


};

#endif
