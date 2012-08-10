cmake_minimum_required ( VERSION 2.8 )
project (kibitz) 
set ( CMAKE_CXX_FLAGS "-g" )

add_library( kibitz SHARED
  kibitz.cpp
  heartbeat_sender.cpp
  context.cpp
  
)

target_link_libraries( kibitz
  boost_program_options
)

add_executable( test-worker-a  test_worker_a.cpp )

target_link_libraries( test-worker-a kibitz )