#ifndef locator_hpp
#define locator_hpp


#include "heartbeat.hpp"
#include <glog/logging.h>
#include <string>

using std::string;

int recv( void* context, string& message, int flag = 0 );

#endif
