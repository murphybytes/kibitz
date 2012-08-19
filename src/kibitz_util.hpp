

#include <zmq.h>
#include <string>
#include <stdexcept>
#include <sstream>
using std::string;
using std::stringstream;
using std::runtime_error;

namespace kibitz {
  namespace util {

    class queue_interrupt : public std::runtime_error {
    public :
      queue_interrupt( const string& msg ) ;
      virtual ~queue_interrupt() throw(); 
    };

    void recv( void* socket, string& message ) ;
    void send( void* socket, const string& message );
    void check_zmq( int zmq_return_code ) ;
    void* create_socket( void* context, int socktype ) ;
    void close_socket( void* socket );
  }
}
