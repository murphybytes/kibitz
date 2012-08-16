

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__


#include "kibitz.hpp"



namespace kibitz {

  class message {
  public:
    virtual string to_json() const = 0;
  };


  typedef shared_ptr<message> message_ptr;

  message_ptr message_factory( const string& json ) ;
}


#endif
