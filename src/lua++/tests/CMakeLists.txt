
file(GLOB srcs test-*.cpp)


add_executable(test-lua++ main.cpp ${srcs})

yack_link_libraries(test-lua++)
target_link_libraries(test-lua++ yack-lua++ yack-lua)

yack_testing(lua++)

enable_testing()

add_test(state test-lua++ state)
