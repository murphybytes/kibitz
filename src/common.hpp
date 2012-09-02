#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <assert.h>

#include <zmq.h>

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector> 
#include <glog/logging.h>

#include <unistd.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <time.h>

#include "constants.hpp"

using std::string;
using std::stringstream;
using std::runtime_error;

using boost::lexical_cast;
using boost::format;
using boost::shared_ptr;
using boost::thread_group;
using boost::property_tree::ptree; 
using boost::dynamic_pointer_cast;
using boost::static_pointer_cast;
using namespace boost::posix_time;

namespace po = boost::program_options;
namespace fs = boost::filesystem3;
namespace pt = boost::property_tree;

using namespace google;
#include "bindings.hpp"


// misc helper functions
namespace kibitz {



  // declared in kibitz.cpp
  class context;
  extern context* context_;

}

#endif 
