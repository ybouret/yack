
file(GLOB base base/*.cpp)
source_group(base FILES ${base} )

file(GLOB memory memory/*.cpp)
source_group(memory FILES ${memory} )

file(GLOB data data/*.cpp)
source_group(data FILES ${data} )

file(GLOB sync sync/*.cpp)
source_group(sync FILES ${sync} )

file(GLOB apex apex/*.cpp)
source_group(apex FILES ${apex} )

file(GLOB fft fft/*.cpp)
source_group(fft FILES ${fft})


add_executable(test-yack main.cpp
${base}
${memory}
${data}
${sync}
${apex}
${fft}
)

yack_link_libraries(test-yack)

enable_testing()
add_test(crc32   test-yack crc32)
add_test(error   test-yack error 0 1 2 3)
add_test(excp    test-yack excp)
add_test(cstring test-yack cstring)
add_test(align   test-yack align)
add_test(ints    test-yack ints)
add_test(base2   test-yack base2)
add_test(at_exit test-yack at_exit)
add_test(wtime   test-yack wtime)

add_test(memory:ram      test-yack ram)
add_test(memory:chunk    test-yack memory_chunk)
add_test(memory:arena    test-yack memory_arena)
add_test(memory:arena_io test-yack memory_arena_io)
add_test(memory:alloc    test-yack memory_alloc)
add_test(memory:book     test-yack memory_book)
add_test(memory:blocks   test-yack memory_blocks)
add_test(memory:note     test-yack memory_note)
add_test(memory:dyadic   test-yack memory_dyadic)
add_test(memory:objects  test-yack memory_objects)
add_test(memory:parcel   test-yack memory_parcel)
add_test(memory:parcels   test-yack memory_parcels)

add_test(object          test-yack object)

add_test(data:list      test-yack data_list)
add_test(data:list:sort test-yack data_list_sort)
add_test(data:pool      test-yack data_pool)
add_test(data:pool:sort test-yack data_pool_sort)

add_test(sync:lockable  test-yack sync_lockable)
add_test(sync:primitive test-yack sync_primitive)
add_test(sync:mutex     test-yack sync_mutex)
add_test(sync:condition test-yack sync_condition)
add_test(sync:thread    test-yack sync_condition)
add_test(singleton      test-yack singleton)

add_test(fft:1d        test-yack  fft1d)

add_test(apex:types     test-yack  apex_types)
add_test(apex:n         test-yack  apn)

yack_testing(yack)
