file(GLOB src *.cpp)

add_executable(test-yack main.cpp ${src})
target_link_libraries(test-yack yack)

enable_testing()
add_test(crc32 test-yack crc32)

yack_testing(yack)
