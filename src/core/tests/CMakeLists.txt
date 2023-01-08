
file(GLOB base base/*.cpp)
source_group(base FILES ${base} )

file(GLOB type type/*.cpp)
source_group(type FILES ${type} )

file(GLOB memory memory/*.cpp)
source_group(memory FILES ${memory} )

file(GLOB data data/*.cpp)
source_group(data FILES ${data} )

file(GLOB dinky dinky/*.cpp)
source_group(dinky FILES ${dinky} )

file(GLOB sync sync/*.cpp)
source_group(sync FILES ${sync} )

file(GLOB arith arith/*.cpp)
source_group(arith FILES ${arith})

file(GLOB apex apex/*.cpp)
source_group(apex FILES ${apex} )

file(GLOB fft fft/*.cpp)
source_group(fft FILES ${fft})

file(GLOB ios ios/*.cpp)
source_group(ios FILES ${ios})

file(GLOB fs fs/*.cpp)
source_group(fs FILES ${fs})

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

file(GLOB math math/*.cpp)
source_group(math FILES ${math})

file(GLOB kr kr/*.cpp)
source_group(kr FILES ${kr})

file(GLOB field field/*.cpp)
source_group(field FILES ${field})

file(GLOB color color/*.cpp)
source_group(color FILES ${color})

file(GLOB raven raven/*.cpp)
source_group(raven FILES ${raven})

set(HERE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test-core main.cpp main.hpp
${base}
${type}
${memory}
${data}
${dinky}
${sync}
${arith}
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
${math}
${fs}
${kr}
${field}
${color}
${raven}
)

yack_link_libraries(test-core)
target_link_libraries(test-core yack-z)
target_link_libraries(test-core yack-bz2)

add_library(dso MODULE dso.cpp)
add_custom_command( TARGET dso
POST_BUILD
COMMAND $<TARGET_FILE:test-core> rtld_dso $<TARGET_FILE:dso>
COMMAND ${CMAKE_COMMAND} -E echo "[YACK] done [DSO]"
DEPENDS test-core
COMMENT "[YACK] test [DSO]")

enable_testing()
add_test(crc32   test-core crc32)
add_test(error   test-core error 0 1 2 3)
add_test(excp    test-core excp)
add_test(cstring test-core cstring)
add_test(align   test-core align)
add_test(base2   test-core base2)
add_test(at_exit test-core at_exit)
add_test(wtime   test-core wtime)
add_test(complex  test-core complex)
add_test(gcd64    test-core gcd64)
add_test(prime    test-core prime)
add_test(primePi  test-core primePi 20)
add_test(prime_stab test-core prime_stab)
add_test(functor  test-core functor)
add_test(rtti     test-core rtti)
add_test(env      test-core env "PATH" nothing)
add_test(div      test-core div)
add_test(zflux    test-core zflux)
add_test(net128   test-core net128)
add_test(abs      test-core abs)
add_test(genp32   test-core genp32)
add_test(prime32  test-core prime32)
add_test(hash64   test-core hash64)
add_test(round10  test-core round10)
add_test(integral test-core integral)


add_test(type:ints   test-core type_ints)
add_test(type:traits test-core type_traits)
add_test(type:temp   test-core type_temp)
add_test(type:conv   test-core type_conv)
add_test(type:scalar test-core type_scalar)
add_test(type:human  test-core type_human)
add_test(type:fourcc test-core type_fourcc)
add_test(type:vtx    test-core type_vtx)
add_test(type:constant test-core type_constant)
add_test(type:gateway  test-core type_gateway)
add_test(type:dec8     test-core type_dec8)


add_test(memory:ram       test-core memory_ram)
add_test(memory:chunk     test-core memory_chunk)
add_test(memory:arena     test-core memory_arena)
add_test(memory:arena_io  test-core memory_arena_io)
add_test(memory:alloc     test-core memory_alloc)
add_test(memory:book      test-core memory_book)
add_test(memory:blocks    test-core memory_blocks)
add_test(memory:note      test-core memory_note)
add_test(memory:dyadic    test-core memory_dyadic)
add_test(memory:objects   test-core memory_objects)
add_test(memory:parcel    test-core memory_parcel)
add_test(memory:parcels   test-core memory_parcels)
add_test(memory:buffers   test-core memory_buffers)
add_test(memory:embed     test-core memory_embed)
add_test(memory:zcache    test-core memory_zcache)
add_test(memory:operative test-core memory_operative)
add_test(memory:ptr       test-core memory_ptr)
add_test(memory:sentry    test-core memory_sentry)
add_test(memory:contractor test-core memory_contractor)
add_test(memory:shelf      test-core memory_shelf)

add_test(object          test-core object)
add_test(large_object    test-core large_object)
add_test(spot_object     test-core spot_object)

add_test(data:list         test-core data_list)
add_test(data:list:sort    test-core data_list_sort)
add_test(data:list:ordered test-core data_list_ordered)
add_test(data:pool         test-core data_pool)
add_test(data:pool:sort    test-core data_pool_sort)
add_test(data:small        test-core data_small)
add_test(data:matrix       test-core data_matrix)
add_test(data:sequence     test-core data_sequence)
add_test(data:cxx_series   test-core data_cxx_series)
add_test(data:heap         test-core data_heap)
add_test(data:tree         test-core data_tree)
add_test(data:hash         test-core data_hash)
add_test(data:iter         test-core data_iter)
add_test(data:key_variety  test-core data_key_variety)
add_test(data:hash_set     test-core data_hash_set)
add_test(data:hash_map     test-core data_hash_map)
add_test(data:suffix_set   test-core data_suffix_set)
add_test(data:suffix_map   test-core data_suffix_map)
add_test(data:registry     test-core data_registry)
add_test(data:glossary     test-core data_glossary)
add_test(data:arrays       test-core data_arrays)
add_test(data:factory      test-core data_factory)
add_test(data:stats        test-core data_stats)
add_test(data:pipes        test-core data_pipes)
add_test(data:addrbook     test-core data_addrbook)
add_test(data:lexicon      test-core data_lexicon)
add_test(data:meta         test-core data_meta)
add_test(data:doubly       test-core data_doubly)
add_test(data:groove       test-core data_groove)
add_test(data:sorted_list  test-core data_sorted_list)
add_test(data:data_be_key  test-core data_be_key)
add_test(data:pair         test-core data_pair)
add_test(data:triple       test-core data_triple)
add_test(data:quad         test-core data_quad)
add_test(data:pqueue       test-core data_pqueue)
add_test(data:roll         test-core data_roll)
add_test(data:bunch        test-core data_bunch)
add_test(data:set          test-core data_set)
add_test(data:joint        test-core data_joint)

add_test(dinky:znone       test-core dinky_znone)
add_test(dinky:zsolo       test-core dinky_zsolo)
add_test(dinky:zcoop       test-core dinky_zcoop)
add_test(dinky:zproxy      test-core dinky_zproxy)
add_test(dinky:registry    test-core dinky_registry)
add_test(dinky:manifest    test-core dinky_manifest)
add_test(dinky:ops         test-core dinky_ops)
add_test(dinky:core:list   test-core dinky_core_list)
add_test(dinky:core:repo   test-core dinky_core_repo)
add_test(dinky:solo:list   test-core dinky_solo_list)
add_test(dinky:solo:repo   test-core dinky_solo_repo)


add_test(sync:lockable  test-core sync_lockable)
add_test(sync:primitive test-core sync_primitive)
add_test(sync:mutex     test-core sync_mutex)
add_test(sync:condition test-core sync_condition)
add_test(sync:thread    test-core sync_condition)
add_test(singleton      test-core singleton)
add_test(sync:loop      test-core sync_loop 4 0.2)
add_test(sync:split     test-core sync_split)
add_test(sync:queue     test-core sync_queue)
add_test(sync:topology  test-core sync_topology)

add_test(arith:isqrt    test-core arith_isqrt)

add_test(fft1d         test-core  fft1d 0.02)
add_test(fft:xbr-info  test-core  fft_xbr_info)
add_test(fft:xbr8      test-core  fft_xbr8  0.02)
add_test(fft:xbr64     test-core  fft_xbr64 0.02)

add_test(apex:mem          test-core  apex_mem)
add_test(apex:info         test-core  apex_info)
add_test(apn               test-core  apn)
add_test(apex:ari          test-core  apex_ari)
add_test(apex:io           test-core  apex_io)
add_test(apz               test-core  apz)
add_test(apq               test-core  apq)
add_test(apex:conv         test-core  apex_conv)
add_test(apfac             test-core  apfac 1 2 3 4 5 10 20)
add_test(aprsa             test-core  aprsa)
add_test(apr               test-core  apr 100 3)
add_test(aprimes           test-core  aprimes ${HERE}/../../../var/1k.txt)
add_test(apex:npd          test-core  apex_npd)
add_test(alga              test-core  alga)
add_test(apex:isqrt        test-core  apex_isqrt)
add_test(aprimes64         test-core  aprimes64)

add_test(ios:c-file     test-core ios_c_file)
add_test(ios:streams    test-core ios_streams)
add_test(ios:gz         test-core ios_gz)
add_test(ios:bz         test-core ios_bz ${HERE}/../../../words.db)
add_test(ios:fmt        test-core ios_fmt)
add_test(ios:gets       test-core ios_gets)
add_test(ios:ascii      test-core ios_ascii)
add_test(ios:hybrid     test-core ios_hybrid)
add_test(ios:convert    test-core ios_convert i9223372036854775807 i-9223372036854775808 u18446744073709551615)
add_test(ios:loader     test-core ios_loader ${HERE}/sample.txt)
add_test(ios:scribe     test-core ios_scribe)
add_test(ios:vstream    test-core ios_vstream)
add_test(ios:pstream    test-core ios_pstream)
add_test(ios:markup     test-core ios_markup)
add_test(ios:bits       test-core ios_bits)
add_test(ios:xmlog      test-core ios_xmlog)

add_test(hashing:64      test-core hash64)
add_test(hashing:fcn     test-core hashing_fcn)
add_test(hashing:hmac    test-core hashing_hmac)
add_test(hashing:key     test-core hashing_key)
add_test(hashing:perfect test-core hashing_perfect ${HERE}/../../../small.db)

add_test(rand:bits      test-core rand_bits)
add_test(rand:metrics   test-core rand_metrics)
add_test(rand:shuffle   test-core rand_shuffle)
add_test(rand:mt19937   test-core rand_mt19937)
add_test(rand:gaussian  test-core rand_gaussian)

add_test(sort:heap      test-core sort_heap)
add_test(sort:indexing  test-core sort_indexing)
add_test(sort:sum       test-core sort_sum)
add_test(sort:nw        test-core sort_nw 0.02)
add_test(sort:network   test-core sort_network)
add_test(sort:quick     test-core sort_quick)
add_test(sort:nws       test-core sort_nws)


add_test(counting:comb   test-core counting_comb)
add_test(counting:perm   test-core counting_perm)
add_test(counting:part   test-core counting_part)
add_test(counting:loop   test-core counting_loop)
add_test(counting:limits test-core counting_limits)
add_test(counting:card   test-core counting_card)
add_test(counting:binary test-core counting_binary)

add_test(rtld:dso test-core rtld_dso)

add_test(string:utf8     test-core string_utf8)
add_test(strings         test-core strings)
add_test(string:tokenize test-core string_tokenize)

add_test(info:bwt      test-core info_bwt)
add_test(info:mod      test-core info_mod)
add_test(info:xbwt     test-core info_xbwt)
add_test(info:alphabet test-core info_alphabet)

add_test(math:api test-core math_api)

add_test(vfs  test-core vfs)
add_test(vfs:local test-core vfs_local)
add_test(vfs:attr  test-core vfs_attr)
add_test(vfs:mkdir test-core vfs_mkdir)
add_test(vfs:bytes test-core vfs_bytes)

add_test(kr:digest   test-core kr_digest)

add_test(fields     test-core fields)

add_test(color:rgb  test-core color_rgb)
add_test(color:yuv  test-core color_yuv)
add_test(color:unit-real test-core color_unit_real)
add_test(color:gs   test-core color_gs)

add_test(raven:space  test-core raven_space)
add_test(raven:family test-core raven_family)

yack_testing(core)
