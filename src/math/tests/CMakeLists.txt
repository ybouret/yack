
file(GLOB algebra algebra/*.cpp)
source_group(algebra FILES ${algebra} )

file(GLOB ode ode/*.cpp)
source_group(ode FILES ${ode} )

add_executable(test-math main.cpp numeric.cpp
${algebra}
${ode}
)

yack_link_libraries(test-math)
target_link_libraries(test-math yack-math)

yack_testing(math)

enable_testing()

add_test(numeric    test-math numeric)
add_test(algebra:lu test-math lu)

add_test(ode:rk4 test-math rk4)
