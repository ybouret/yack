
file(GLOB srcs test-*.cpp)

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test-lua++ main.cpp ${srcs} sample.lua)

yack_link_libraries(test-lua++)
target_link_libraries(test-lua++ yack-lua++ yack-lua)

yack_testing(lua++)

enable_testing()

add_test(state    test-lua++ state ${HERE}/sample.lua)
add_test(function test-lua++ function)
