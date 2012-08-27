cmake_minimum_required ( VERSION 2.8 )
project (kibitz) 
set ( CMAKE_CXX_FLAGS "-g" )

add_library( kibitz_messages SHARED 
  heartbeat.cpp
  message.cpp
  inproc_notification_message.cpp
  worker_notification_message.cpp
  worker_query.cpp
  worker_query_response.cpp
  worker_broadcast_message.cpp
)


add_library( kibitz SHARED
  kibitz.cpp
  heartbeat_sender.cpp
  context.cpp
  message_base.cpp
  common.cpp
  heartbeat_receiver.cpp
  kibitz_util.cpp
  worker_map.cpp
  bus.cpp
  in_edge_manager.cpp
  basic_collaboration_message.cpp
)

add_library( locator SHARED
  registry.cpp
  kibitz_util.cpp
)

target_link_libraries( kibitz_messages 
  boost_program_options-mt
  boost_system-mt
)

target_link_libraries( kibitz
  kibitz_messages		       
  boost_program_options-mt
  boost_filesystem-mt
  boost_system-mt
  boost_thread-mt
  glog
  zmq
  yaml-cpp  
)

target_link_libraries( locator
  kibitz_messages 
  boost_program_options-mt
  boost_filesystem-mt
  boost_system-mt
  boost_thread-mt
  glog
  zmq
)




add_executable( test-worker-a  test_worker_a.cpp )
add_executable( unit-test unit_test.cpp )
add_executable( worker-locator locator.cpp ) 


target_link_libraries( test-worker-a kibitz )
target_link_libraries( unit-test kibitz )
target_link_libraries( worker-locator locator )

