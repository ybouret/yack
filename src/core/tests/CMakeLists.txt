file(GLOB src *.cpp)

add_executable(test-yack main.cpp ${src})
target_link_libraries(test-yack yack)

enable_testing()
add_test(crc32   test-yack crc32)
add_test(error   test-yack error 0 1 2 3)
add_test(excp    test-yack excp)
add_test(cstring test-yack cstring)
add_test(align   test-yack align)
add_test(ints    test-yack ints)
add_test(base2   test-yack base2)

add_test(memory:ram     test-yack ram)

yack_testing(yack)
