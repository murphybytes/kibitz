#include "kibitz.hpp"




int main( int argc, char* argv[] ) {
  std::cout << "starting" << std::endl;
  int result = 0;

  try {
    kibitz::initialize( argc, argv );
    kibitz::start();
    kibitz::terminate();
  } catch( std::exception& e ) {
    result = 1;
    std::cout << "Program failed. " << e.what() << std::endl;
  }
  
  return result;


}
