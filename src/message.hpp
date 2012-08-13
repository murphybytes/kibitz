// TODO: abstract base for all messages


#include "kibitz.hpp"



namespace kibitz {

  class message {
  public:
    virtual string to_json() const = 0;
  };


  typedef shared_ptr<message> message_ptr;

  message_ptr message_factory( const string& json ) ;
}
