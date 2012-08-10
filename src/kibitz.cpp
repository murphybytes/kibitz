#include "kibitz.hpp"
#include "heartbeat_sender.hpp"
#include "context.hpp"


namespace kibitz {
  static context* context_ptr = NULL;

  void initialize( int argc, char* argv[] ) {
    assert( NULL == context_ptr );
    //context_ptr = new context( application_configuration );
  }

  

  void terminate() {
    delete context_ptr;
  }


}

