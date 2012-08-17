

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
  public:
    virtual string to_json() const = 0;

  };


  typedef shared_ptr<message> message_ptr;

  message_ptr message_factory( const string& json ) ;
}


#endif
