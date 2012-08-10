#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <assert.h>

#include <zmq.h>

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include <glog/logging.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

using std::string;
using std::stringstream;
using std::runtime_error;

using boost::format;

namespace po = boost::program_options;
namespace fs = boost::filesystem3;

using namespace google;

#endif 
