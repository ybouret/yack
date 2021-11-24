
file(GLOB algebra algebra/*.cpp)
source_group(algebra FILES ${algebra} )

add_executable(test-math main.cpp numeric.cpp
${algebra}
)

yack_link_libraries(test-math)
target_link_libraries(test-math yack-math)

yack_testing(math)

enable_testing()

add_test(numeric    test-math numeric)
add_test(algebra:lu test-math lu)

