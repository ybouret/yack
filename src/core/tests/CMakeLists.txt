
file(GLOB base base/*.cpp)
source_group(base FILES ${base} )

file(GLOB memory memory/*.cpp)
source_group(memory FILES ${memory} )

file(GLOB data data/*.cpp)
source_group(data FILES ${data} )

file(GLOB sync sync/*.cpp)
source_group(sync FILES ${sync} )


add_executable(test-yack main.cpp
${base}
${memory}
${data}
${sync}
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
add_test(at_exit test-yack at_exit)

add_test(memory:ram    test-yack ram)
add_test(memory:chunk  test-yack memory_chunk)
add_test(memory:arena  test-yack memory_arena)

add_test(data:list      test-yack data_list)

add_test(sync:lockable  test-yack sync_lockable)

yack_testing(yack)
