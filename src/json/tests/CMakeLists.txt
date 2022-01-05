
file(GLOB srcs test-*.cpp)
  
 

add_executable(test-json main.cpp ${srcs})

target_link_libraries(test-json yack-json yack-jive)
yack_link_libraries(test-json)

yack_testing(json)

enable_testing()

add_test(parser test-json parser)






