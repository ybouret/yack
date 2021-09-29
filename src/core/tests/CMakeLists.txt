file(GLOB src *.cpp)

add_executable(test-yack main.cpp ${src})
target_link_libraries(test-yack yack)

enable_testing()
add_test(crc32   test-yack crc32)
add_test(error   test-yack error 0 1 2 3)
add_test(cstring test-yack cstring)

yack_testing(yack)
