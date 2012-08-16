
#ifndef __COLLABORATION_MESSAGE_HPP__
#define __COLLABORATION-MESSAGE_HPP__

#include  "message.hpp"

namespace kibitz {
  class collaboration_message : public message {
  public:
    virtual string to_json() const = 0;
  };
}

#endif



