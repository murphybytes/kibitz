#include "kibitz.hpp"




int main( int argc, char* argv[] ) {
  std::cout << "starting" << std::endl;
  variables_map application_configuration ;
  kibitz::initialize( application_configuration );
  
  return 0;


}
