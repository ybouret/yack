
file(GLOB global global/*.cpp)
source_group(global FILES ${global} )

file(GLOB algebra algebra/*.cpp)
source_group(algebra FILES ${algebra} )

file(GLOB root root/*.cpp)
source_group(root FILES ${root} )

file(GLOB ode ode/*.cpp)
source_group(ode FILES ${ode} )

add_executable(test-math main.cpp
${global}
${algebra}
${ode}
${root}
)

yack_link_libraries(test-math)
target_link_libraries(test-math yack-math)

yack_testing(math)

enable_testing()

add_test(numeric    test-math numeric)
add_test(triplet    test-math triplet)
add_test(algebra:lu test-math lu)

add_test(ode:rk4 test-math rk4)

add_test(root:zalg test-math root_zalg)
