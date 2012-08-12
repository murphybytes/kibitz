#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <assert.h>

#include <zmq.h>

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include <glog/logging.h>

#include <unistd.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "constants.hpp"

using std::string;
using std::stringstream;
using std::runtime_error;

using boost::format;
using boost::shared_ptr;
using boost::thread_group;


namespace po = boost::program_options;
namespace fs = boost::filesystem3;

using namespace google;

// misc helper functions
namespace kibitz {

  void recv( void* socket, string& message  ) ;
  void send( void* socket, const string& message ) ;
}

#endif 
