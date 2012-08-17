

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <sstream>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <string>

using boost::property_tree::ptree;
using boost::shared_ptr;
using std::string;
using std::stringstream;

namespace kibitz {
  

  class message {
    const string message_class_;
  protected :
    message( const string& message_class )
      :message_class_(message_class) {}
  public:
    static const int stop = 0;
    virtual string to_json() const = 0;
    const string& message_class() const { return message_class_; }
  };


  typedef shared_ptr<message> message_ptr;

  message_ptr message_factory( const string& json ) ;
}


#endif
