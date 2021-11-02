
file(GLOB base base/*.cpp)
source_group(base FILES ${base} )

file(GLOB type type/*.cpp)
source_group(type FILES ${type} )

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

file(GLOB ios ios/*.cpp)
source_group(ios FILES ${ios})


file(GLOB hashing hashing/*.cpp)
source_group(hashing FILES ${hashing})

file(GLOB rand rand/*.cpp)
source_group(rand FILES ${rand})

file(GLOB sort sort/*.cpp)
source_group(sort FILES ${sort})

file(GLOB counting counting/*.cpp)
source_group(counting FILES ${counting})

file(GLOB rtld rtld/*.cpp)
source_group(rtld FILES ${rtld})

file(GLOB info info/*.cpp)
source_group(info FILES ${info})

file(GLOB string string/*.cpp)
source_group(string FILES ${string})

add_executable(test-yack main.cpp
${base}
${type}
${memory}
${data}
${sync}
${apex}
${fft}
${ios}
${hashing}
${rand}
${sort}
${rtld}
${counting}
${string}
${info}
)

yack_link_libraries(test-yack)
target_link_libraries(test-yack yack-z)

add_library(dso MODULE dso.cpp)
add_custom_command( TARGET dso
POST_BUILD
COMMAND $<TARGET_FILE:test-yack> rtld_dso $<TARGET_FILE:dso>
DEPENDS test-yack
COMMENT "[YACK]  <testing DSO/>")

enable_testing()
add_test(crc32   test-yack crc32)
add_test(error   test-yack error 0 1 2 3)
add_test(excp    test-yack excp)
add_test(cstring test-yack cstring)
add_test(align   test-yack align)
add_test(base2   test-yack base2)
add_test(at_exit test-yack at_exit)
add_test(wtime   test-yack wtime)
add_test(complex  test-yack complex)
add_test(gcd64    test-yack gcd64)

add_test(type:ints   test-yack type_ints)
add_test(type:traits test-yack type_traits)
add_test(type:temp   test-yack type_temp)
add_test(type:conv   test-yack type_conv)
add_test(type:scalar test-yack type_scalar)
add_test(type:human  test-yack type_human)
add_test(type:fourcc test-yack type_fourcc)
add_test(type:vtx    test-yack type_vtx)


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
add_test(memory:parcels  test-yack memory_parcels)
add_test(memory:buffers  test-yack memory_buffers)
add_test(memory:embed    test-yack memory_embed)
add_test(memory:zcache   test-yack memory_zcache)

add_test(object          test-yack object)

add_test(data:list      test-yack data_list)
add_test(data:list:sort test-yack data_list_sort)
add_test(data:pool      test-yack data_pool)
add_test(data:pool:sort test-yack data_pool_sort)
add_test(data:small     test-yack data_small)
add_test(data:matrix    test-yack data_matrix)
add_test(data:sequence  test-yack data_sequence)

add_test(sync:lockable  test-yack sync_lockable)
add_test(sync:primitive test-yack sync_primitive)
add_test(sync:mutex     test-yack sync_mutex)
add_test(sync:condition test-yack sync_condition)
add_test(sync:thread    test-yack sync_condition)
add_test(singleton      test-yack singleton)
add_test(sync:loop      test-yack sync_loop 4)
add_test(sync:split     test-yack sync_split)
add_test(sync:queue     test-yack sync_queue)

add_test(fft:1d        test-yack  fft1d 0.1)
add_test(fft:xbr-info  test-yack  fft_xbr_info)
add_test(fft:xbr8      test-yack  fft_xbr8  0.05)
add_test(fft:xbr64     test-yack  fft_xbr64 0.05)

add_test(apex:mem       test-yack  apex_mem)
add_test(apex:n         test-yack  apn)
add_test(apex:ari       test-yack  apex_ari)
add_test(apex:io        test-yack  apex_io)

add_test(ios::c-file    test-yack ios_c_file)
add_test(ios:streams    test-yack ios_streams)
add_test(ios:gz         test-yack ios_gz)
add_test(ios:fmt        test-yack ios_fmt)
add_test(ios:gets       test-yack ios_gets)

add_test(hashing::64    test-yack hash64)

add_test(rand:bits      test-yack rand_bits)

add_test(sort:heap      test-yack sort_heap)
add_test(sort:indexing  test-yack sort_indexing)

add_test(counting:comb  test-yack counting_comb)
add_test(counting:perm  test-yack counting_perm)
add_test(counting:part  test-yack counting_part)

add_test(rtld:dso test-yack rtld_dso)

add_test(string:utf8 test-yack string_utf8)

add_test(info:bwt  test-yack info_bwt)
add_test(info:mod  test-yack info_mod)
add_test(info:xbwt test-yack info_xbwt)


yack_testing(yack)
