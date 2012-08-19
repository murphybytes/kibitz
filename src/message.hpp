

#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <sstream>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <string>

#include <stdint.h>

using boost::property_tree::ptree;
using boost::shared_ptr;
using std::string;
using std::stringstream;

namespace kibitz {
  

  class message {
    const string message_type_;
  protected :
    message( const string& message_type )
      :message_type_(message_type) {}
    virtual void populate_header( ptree& tree ) const {
      tree.put( "message_type", message_type_ );
    }
  public:
    static const int16_t PORT_UNASSIGNED = 0x7FFF;
    static const int32_t WORKER_ID_UNASSIGNED = 0x7FFFFFFFL;

    virtual ~message() {}
    static const int stop = 0;
    virtual string to_json() const = 0;
    const string& message_type() const { return message_type_; }
  };


  typedef shared_ptr<message> message_ptr;

  message_ptr message_factory( const string& json ) ;
}


#endif
