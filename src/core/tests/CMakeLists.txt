
file(GLOB base base/test-*.cpp)
source_group(base FILES ${base} )


file(GLOB memory memory/test-*.cpp)
source_group(memory FILES ${memory} )

add_executable(test-yack main.cpp
${base}
${memory}
)
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
add_test(memory:chunk  test-yack memory_chunk)


yack_testing(yack)
