#ifndef bus_hpp
#define bus_hpp

#include "kibitz.hpp"


namespace kibitz {

  class bus {

  protected:
    void* socket_;   
    bus( void* zmq_context, const char* binding, int sock_type ) ;
    virtual ~bus();
  public:
    void* socket() { return socket_; }
  };

  class pub : public bus {
  public: 
    pub( void* zmq_context, const char* binding );
    virtual ~pub(){}
    void send( const string& json ); 
  };
  
  class sub : public bus {
  public: 
    sub( void* zmq_context, const char* binding ) ;
    virtual ~sub() {}

    
  };

}

#endif
