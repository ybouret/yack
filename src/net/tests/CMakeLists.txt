
file(GLOB srcs test-*.cpp)
add_executable(test-net main.cpp ${srcs})

target_link_libraries(test-net yack-net)
yack_link_libraries(test-net)

if(YACK_GNU AND WIN32)
	target_link_libraries(test-net ws2_32)
endif()

if(YACK_SUNOS)
	target_link_libraries(test-net socket)
endif()



yack_testing(net)

enable_testing()

add_test(types      test-net types)
add_test(init       test-net init)
add_test(socket     test-net socket)
add_test(addr       test-net addr)
add_test(buffer     test-net buffer)
add_test(resolve    test-net resolve localhost)
add_test(tcp_client test-net tcp_client)
add_test(tcp_server test-net tcp_server)
add_test(udp_client test-net udp_client)
add_test(udp_server test-net udp_server)

