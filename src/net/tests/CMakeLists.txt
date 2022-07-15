
file(GLOB srcs test-*.cpp)
add_executable(test-net main.cpp ${srcs})

target_link_libraries(test-net yack-net)
yack_link_libraries(test-net)

yack_testing(net)

enable_testing()

add_test(types test-net types)







