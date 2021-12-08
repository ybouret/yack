project(core C CXX)

include_directories(.)

########################################################################
##
## top-level files
##
########################################################################
file(GLOB src_impl yack/*.c yack/*.cpp)
file(GLOB src_decl yack/*.h yack/*.hpp)
set(src ${src_impl} ${src_decl})
install( FILES ${src_decl} DESTINATION include/yack )

########################################################################
##
## type operation
##
########################################################################
file(GLOB type_impl yack/type/*.cpp yack/type/*.c)
file(GLOB type_decl yack/type/*.hpp yack/type/*.h)
set(type ${type_impl} ${type_decl})
SOURCE_GROUP(type FILES ${type})
install( FILES ${type_decl} DESTINATION include/yack/type )

########################################################################
##
## arith operation
##
########################################################################
file(GLOB arith_impl yack/arith/*.cpp yack/arith/*.c)
file(GLOB arith_decl yack/arith/*.hpp yack/arith/*.h)
set(arith ${arith_impl} ${arith_decl})
SOURCE_GROUP(arith FILES ${arith})
install( FILES ${arith_decl} DESTINATION include/yack/arith )

########################################################################
##
## checking operations
##
########################################################################
file(GLOB check_impl yack/check/*.cpp yack/check/*.c)
file(GLOB check_decl yack/check/*.hpp yack/check/*.h)
set(check ${check_impl} ${check_decl})
SOURCE_GROUP(check FILES ${check})
install( FILES ${check_decl} DESTINATION include/yack/check )

########################################################################
##
## system operations
##
########################################################################
file(GLOB system_impl yack/system/*.cpp yack/system/*.c)
file(GLOB system_decl yack/system/*.hpp yack/system/*.h)
set(system ${system_impl} ${system_decl})
SOURCE_GROUP(system FILES ${system})
install( FILES ${system_decl} DESTINATION include/yack/system )

########################################################################
##
## unit tests structure
##
########################################################################
file(GLOB utest_impl yack/utest/*.cpp)
file(GLOB utest_decl yack/utest/*.hpp)
set(utest ${utest_impl} ${utest_decl})
SOURCE_GROUP(utest FILES ${utest})
install( FILES ${utest_decl} DESTINATION include/yack/utest )

########################################################################
##
## data structure operations
##
########################################################################
file(GLOB data_impl yack/data/*.cpp yack/data/*.c)
file(GLOB data_decl yack/data/*.hpp yack/data/*.h)
set(data ${data_impl} ${data_decl})
SOURCE_GROUP(data FILES ${data})
install( FILES ${data_decl} DESTINATION include/yack/data )

file(GLOB data_small_impl yack/data/small/*.cpp)
file(GLOB data_small_decl yack/data/small/*.hpp)
set(data_small ${data_small_impl} ${data_small_decl})
SOURCE_GROUP(data/small FILES ${data_small})
install( FILES ${data_small_decl} DESTINATION include/yack/data/small )


file(GLOB data_list_impl yack/data/list/*.cpp)
file(GLOB data_list_decl yack/data/list/*.hpp)
set(data_list ${data_list_impl} ${data_list_decl})
SOURCE_GROUP(data/list FILES ${data_list})
install( FILES ${data_list_decl} DESTINATION include/yack/data/list )

file(GLOB data_pool_impl yack/data/pool/*.cpp)
file(GLOB data_pool_decl yack/data/pool/*.hpp)
set(data_pool ${data_pool_impl} ${data_pool_decl})
SOURCE_GROUP(data/pool FILES ${data_pool})
install( FILES ${data_pool_decl} DESTINATION include/yack/data/pool )

file(GLOB data_suffix_impl yack/data/suffix/*.cpp)
file(GLOB data_suffix_decl yack/data/suffix/*.hpp)
set(data_suffix ${data_suffix_impl} ${data_suffix_decl})
SOURCE_GROUP(data/suffix FILES ${data_suffix})
install( FILES ${data_suffix_decl} DESTINATION include/yack/data/suffix )

file(GLOB data_hash_impl yack/data/hash/*.cpp)
file(GLOB data_hash_decl yack/data/hash/*.hpp)
set(data_hash ${data_hash_impl} ${data_hash_decl})
SOURCE_GROUP(data/hash FILES ${data_hash})
install( FILES ${data_hash_decl} DESTINATION include/yack/data/hash )


########################################################################
##
## container   
##
########################################################################
file(GLOB container_impl yack/container/*.cpp yack/container/*.c)
file(GLOB container_decl yack/container/*.hpp yack/container/*.h)
set(container ${container_impl} ${container_decl})
SOURCE_GROUP(container FILES ${container})
install( FILES ${container_decl} DESTINATION include/yack/container )

file(GLOB container_matrix_impl yack/container/matrix/*.cpp)
file(GLOB container_matrix_decl yack/container/matrix/*.hpp)
set(container_matrix ${container_matrix_impl} ${container_matrix_decl})
SOURCE_GROUP(container/matrix FILES ${container_matrix})
install( FILES ${container_matrix_decl} DESTINATION include/yack/container/matrix )

file(GLOB container_iterator_impl yack/container/iterator/*.cpp)
file(GLOB container_iterator_decl yack/container/iterator/*.hpp)
set(container_iterator ${container_iterator_impl} ${container_iterator_decl})
SOURCE_GROUP(container/iterator FILES ${container_iterator})
install( FILES ${container_iterator_decl} DESTINATION include/yack/container/iterator )


########################################################################
##
## sequence(s)   
##
########################################################################
file(GLOB sequence_impl yack/sequence/*.cpp)
file(GLOB sequence_decl yack/sequence/*.hpp)
set(sequence ${sequence_impl} ${sequence_decl})
SOURCE_GROUP(sequence FILES ${sequence})
install( FILES ${sequence_decl} DESTINATION include/yack/sequence )


########################################################################
##
## associative(s)   
##
########################################################################
file(GLOB associative_impl yack/associative/*.cpp)
file(GLOB associative_decl yack/associative/*.hpp)
set(associative ${associative_impl} ${associative_decl})
SOURCE_GROUP(associative FILES ${associative})
install( FILES ${associative_decl} DESTINATION include/yack/associative )

file(GLOB associative_hash_impl yack/associative/hash/*.cpp)
file(GLOB associative_hash_decl yack/associative/hash/*.hpp)
set(associative_hash ${associative_hash_impl} ${associative_hash_decl})
SOURCE_GROUP(associative/hash FILES ${associative_hash})
install( FILES ${associative_hash_decl} DESTINATION include/yack/associative/hash )


file(GLOB associative_suffix_impl yack/associative/suffix/*.cpp)
file(GLOB associative_suffix_decl yack/associative/suffix/*.hpp)
set(associative_suffix ${associative_suffix_impl} ${associative_suffix_decl})
SOURCE_GROUP(associative/suffix FILES ${associative_suffix})
install( FILES ${associative_suffix_decl} DESTINATION include/yack/associative/suffix )

########################################################################
##
## functor(s)   
##
########################################################################
file(GLOB functor_impl yack/functor/*.cpp)
file(GLOB functor_decl yack/functor/*.hpp)
set(functor ${functor_impl} ${functor_decl})
SOURCE_GROUP(functor FILES ${functor})
install( FILES ${functor_decl} DESTINATION include/yack/functor )

#########################################################################
##
## memory operations
##
########################################################################
file(GLOB memory_impl yack/memory/*.cpp)
file(GLOB memory_decl yack/memory/*.hpp)
set(memory ${memory_impl} ${memory_decl})
SOURCE_GROUP(memory FILES ${memory})
install( FILES ${memory_decl} DESTINATION include/yack/memory )
 
# memory small 
file(GLOB memory_small_impl yack/memory/small/*.cpp)
file(GLOB memory_small_decl yack/memory/small/*.hpp)
set(memory_small ${memory_small_impl} ${memory_small_decl})
SOURCE_GROUP(memory/small FILES ${memory_small})
install( FILES ${memory_small_decl} DESTINATION include/yack/memory/small )
 
# memory large 
file(GLOB memory_large_impl yack/memory/large/*.cpp)
file(GLOB memory_large_decl yack/memory/large/*.hpp)
set(memory_large ${memory_large_impl} ${memory_large_decl})
SOURCE_GROUP(memory/large FILES ${memory_large})
install( FILES ${memory_large_decl} DESTINATION include/yack/memory/large )
 
# memory joint 
file(GLOB memory_joint_impl yack/memory/joint/*.cpp)
file(GLOB memory_joint_decl yack/memory/joint/*.hpp)
set(memory_joint ${memory_joint_impl} ${memory_joint_decl})
SOURCE_GROUP(memory/joint FILES ${memory_joint})
install( FILES ${memory_joint_decl} DESTINATION include/yack/memory/joint )
 
# memory allocators 
file(GLOB memory_allocator_impl yack/memory/allocator/*.cpp)
file(GLOB memory_allocator_decl yack/memory/allocator/*.hpp)
set(memory_allocator ${memory_allocator_impl} ${memory_allocator_decl})
SOURCE_GROUP(memory/allocator FILES ${memory_allocator})
install( FILES ${memory_allocator_decl} DESTINATION include/yack/memory/allocator )
 
# memory buffers 
file(GLOB memory_buffer_impl yack/memory/buffer/*.cpp)
file(GLOB memory_buffer_decl yack/memory/buffer/*.hpp)
set(memory_buffer ${memory_buffer_impl} ${memory_buffer_decl})
SOURCE_GROUP(memory/buffer FILES ${memory_buffer})
install( FILES ${memory_buffer_decl} DESTINATION include/yack/memory/buffer )
 
 
########################################################################
##
## fft operations
##
########################################################################
set(YACK_CORE ${CMAKE_CURRENT_SOURCE_DIR})
set(YACK_FFT  ${YACK_CORE}/yack/fft)


if(FALSE)
add_executable(xbitrev yack/fft/xbitrev.cxx yack/fft/xbr-defs.hpp)
add_custom_command(
OUTPUT  ${YACK_FFT}/xbr-decl.hxx ${YACK_FFT}/xbr-impl.cxx
COMMAND xbitrev ${YACK_FFT}/xbr-decl.hxx ${YACK_FFT}/xbr-impl.cxx
COMMENT "[YACK] Creating xbitrev code"
DEPENDS xbitrev
VERBATIM
)
endif()

set(fft_decl
${YACK_FFT}/fft1d.hpp
${YACK_FFT}/xbitrev.hpp
${YACK_FFT}/xbr-decl.hxx
${YACK_FFT}/xbr-defs.hpp
)
set(fft_impl
${YACK_FFT}/fft1d.cpp
${YACK_FFT}/xbr-impl.cxx
)
set(fft ${fft_impl} ${fft_decl})
SOURCE_GROUP(fft FILES ${fft})
install( FILES ${fft_decl} DESTINATION include/yack/fft )  
 
########################################################################
##
## apex operations
##
########################################################################
file(GLOB apex_impl yack/apex/*.cpp)
file(GLOB apex_decl yack/apex/*.hpp)
set(apex ${apex_impl} ${apex_decl})
SOURCE_GROUP(apex FILES ${apex})
install( FILES ${apex_decl} DESTINATION include/yack/apex ) 
 

file(GLOB apex_m_impl yack/apex/m/*.cpp)
file(GLOB apex_m_decl yack/apex/m/*.hpp)
set(apex_m ${apex_m_impl} ${apex_m_decl})
SOURCE_GROUP(apex/m FILES ${apex_m})
install( FILES ${apex_m_decl} DESTINATION include/yack/apex/m ) 

file(GLOB apex_n_impl yack/apex/n/*.cpp)
file(GLOB apex_n_decl yack/apex/n/*.hpp)
set(apex_n ${apex_n_impl} ${apex_n_decl})
SOURCE_GROUP(apex/n FILES ${apex_n})
install( FILES ${apex_n_decl} DESTINATION include/yack/apex/n ) 

file(GLOB apex_z_impl yack/apex/z/*.cpp)
file(GLOB apex_z_decl yack/apex/z/*.hpp)
set(apex_z ${apex_z_impl} ${apex_z_decl})
SOURCE_GROUP(apex/z FILES ${apex_z})
install( FILES ${apex_z_decl} DESTINATION include/yack/apex/z ) 

file(GLOB apex_q_impl yack/apex/q/*.cpp)
file(GLOB apex_q_decl yack/apex/q/*.hpp)
set(apex_q ${apex_q_impl} ${apex_q_decl})
SOURCE_GROUP(apex/q FILES ${apex_q})
install( FILES ${apex_q_decl} DESTINATION include/yack/apex/q ) 

########################################################################
##
## ios
##
########################################################################
file(GLOB ios_impl yack/ios/*.cpp yack/ios/*.c)
file(GLOB ios_decl yack/ios/*.hpp yack/ios/*.h)
set(ios ${ios_impl} ${ios_decl})
SOURCE_GROUP(ios FILES ${ios})
install( FILES ${ios_decl} DESTINATION include/yack/ios )

file(GLOB ios_c_impl yack/ios/c/*.cpp)
file(GLOB ios_c_decl yack/ios/c/*.hpp)
set(ios_c ${ios_c_impl} ${ios_c_decl})
SOURCE_GROUP(ios/c FILES ${ios_c})
install( FILES ${ios_c_decl} DESTINATION include/yack/ios/c)

file(GLOB ios_gz_impl yack/ios/gz/*.cpp)
file(GLOB ios_gz_decl yack/ios/gz/*.hpp)
set(ios_gz ${ios_gz_impl} ${ios_gz_decl})
SOURCE_GROUP(ios/gz FILES ${ios_gz})
install( FILES ${ios_gz_decl} DESTINATION include/yack/ios/gz)

file(GLOB ios_fmt_impl yack/ios/fmt/*.cpp)
file(GLOB ios_fmt_decl yack/ios/fmt/*.hpp)
set(ios_fmt ${ios_fmt_impl} ${ios_fmt_decl})
SOURCE_GROUP(ios/fmt FILES ${ios_fmt})
install( FILES ${ios_fmt_decl} DESTINATION include/yack/ios/fmt)

file(GLOB ios_gv_impl yack/ios/gv/*.cpp)
file(GLOB ios_gv_decl yack/ios/gv/*.hpp)
set(ios_gv ${ios_gv_impl} ${ios_gv_decl})
SOURCE_GROUP(ios/gv FILES ${ios_gv})
install( FILES ${ios_gv_decl} DESTINATION include/yack/ios/gv)

file(GLOB ios_ascii_impl yack/ios/ascii/*.cpp yack/ios/ascii/*.hxx)
file(GLOB ios_ascii_decl yack/ios/ascii/*.hpp)
set(ios_ascii ${ios_ascii_impl} ${ios_ascii_decl})
SOURCE_GROUP(ios/ascii FILES ${ios_ascii})
install( FILES ${ios_ascii_decl} DESTINATION include/yack/ios/ascii)

file(GLOB ios_serializer_impl yack/ios/serializer/*.cpp yack/ios/serializer/*.hxx)
file(GLOB ios_serializer_decl yack/ios/serializer/*.hpp)
set(ios_serializer ${ios_serializer_impl} ${ios_serializer_decl})
SOURCE_GROUP(ios/serializer FILES ${ios_serializer})
install( FILES ${ios_serializer_decl} DESTINATION include/yack/ios/serializer)

########################################################################
##
## kr
##
########################################################################
file(GLOB kr_impl yack/kr/*.cpp)
file(GLOB kr_decl yack/kr/*.hpp)
set(kr ${kr_impl} ${kr_decl})
SOURCE_GROUP(kr FILES ${kr})
install( FILES ${kr_decl} DESTINATION include/yack/kr )

########################################################################
##
## fs
##
########################################################################
file(GLOB fs_impl yack/fs/*.cpp)
file(GLOB fs_decl yack/fs/*.hpp)
set(fs ${fs_impl} ${fs_decl})
SOURCE_GROUP(fs FILES ${fs})
install( FILES ${fs_decl} DESTINATION include/yack/fs )

file(GLOB fs_local_impl yack/fs/local/*.cpp)
file(GLOB fs_local_decl yack/fs/local/*.hpp)
set(fs_local ${fs_local_impl} ${fs_local_decl})
SOURCE_GROUP(fs/local FILES ${fs_local})
install( FILES ${fs_local_decl} DESTINATION include/yack/fs/local )

########################################################################
##
## hashing
##
########################################################################
file(GLOB hashing_impl yack/hashing/*.cpp yack/hashing/*.c)
file(GLOB hashing_decl yack/hashing/*.hpp yack/hashing/*.h)
set(hashing ${hashing_impl} ${hashing_decl})
SOURCE_GROUP(hashing FILES ${hashing})
install( FILES ${hashing_decl} DESTINATION include/yack/hashing )

########################################################################
##
## info
##
########################################################################
file(GLOB info_impl yack/info/*.cpp yack/info/*.c)
file(GLOB info_decl yack/info/*.hpp yack/info/*.h)
set(info ${info_impl} ${info_decl})
SOURCE_GROUP(info FILES ${info})
install( FILES ${info_decl} DESTINATION include/yack/info )

file(GLOB info_modulation_impl yack/info/modulation/*.cpp)
file(GLOB info_modulation_decl yack/info/modulation/*.hpp)
set(info_modulation ${info_modulation_impl} ${info_modulation_decl})
SOURCE_GROUP(info/modulation FILES ${info_modulation})
install( FILES ${info_modulation_decl} DESTINATION include/yack/info/modulation )

########################################################################
##
## randomized
##
########################################################################
file(GLOB randomized_impl yack/randomized/*.cpp)
file(GLOB randomized_decl yack/randomized/*.hpp)
set(randomized ${randomized_impl} ${randomized_decl})
SOURCE_GROUP(randomized FILES ${randomized})
install( FILES ${randomized_decl} DESTINATION include/yack/randomized )

########################################################################
##
## sort
##
########################################################################
file(GLOB sort_impl yack/sort/*.cpp yack/sort/*.c)
file(GLOB sort_decl yack/sort/*.hpp yack/sort/*.h)
set(sort ${sort_impl} ${sort_decl})
SOURCE_GROUP(sort FILES ${sort})
install( FILES ${sort_decl} DESTINATION include/yack/sort )

########################################################################
##
## counting
##
########################################################################
file(GLOB counting_impl yack/counting/*.cpp)
file(GLOB counting_decl yack/counting/*.hpp)
set(counting ${counting_impl} ${counting_decl})
SOURCE_GROUP(counting FILES ${counting})
install( FILES ${counting_decl} DESTINATION include/yack/counting )

file(GLOB counting_c_impl yack/counting/c/*.c)
file(GLOB counting_c_decl yack/counting/c/*.h)
set(counting_c ${counting_c_impl} ${counting_c_decl})
SOURCE_GROUP(counting/c FILES ${counting_c})
install( FILES ${counting_c_decl} DESTINATION include/yack/counting/c )

########################################################################
##
## concurrent operations
##
########################################################################
file(GLOB concurrent_impl yack/concurrent/*.cpp)
file(GLOB concurrent_decl yack/concurrent/*.hpp)
set(concurrent ${concurrent_impl} ${concurrent_decl})
SOURCE_GROUP(concurrent FILES ${concurrent})
install( FILES ${concurrent_decl} DESTINATION include/yack/concurrent )
 
# concurrent/quark operations
file(GLOB concurrent_quark_impl yack/concurrent/quark/*.cpp yack/concurrent/quark/*.hxx)
file(GLOB concurrent_quark_decl yack/concurrent/quark/*.hpp)
set(concurrent_quark ${concurrent_quark_impl} ${concurrent_quark_decl})
SOURCE_GROUP(concurrent/quark FILES ${concurrent_quark})
install( FILES ${concurrent_quark_decl} DESTINATION include/yack/concurrent/quark )

# concurrent/loop operations
file(GLOB concurrent_loop_impl yack/concurrent/loop/*.cpp)
file(GLOB concurrent_loop_decl yack/concurrent/loop/*.hpp)
set(concurrent_loop ${concurrent_loop_impl} ${concurrent_loop_decl})
SOURCE_GROUP(concurrent/loop FILES ${concurrent_loop})
install( FILES ${concurrent_loop_decl} DESTINATION include/yack/concurrent/loop )

# concurrent/queue operations
file(GLOB concurrent_queue_impl yack/concurrent/queue/*.cpp)
file(GLOB concurrent_queue_decl yack/concurrent/queue/*.hpp)
set(concurrent_queue ${concurrent_queue_impl} ${concurrent_queue_decl})
SOURCE_GROUP(concurrent/queue FILES ${concurrent_queue})
install( FILES ${concurrent_queue_decl} DESTINATION include/yack/concurrent/queue )

########################################################################
##
## rtld
##
########################################################################
file(GLOB rtld_impl yack/rtld/*.cpp)
file(GLOB rtld_decl yack/rtld/*.hpp)
set(rtld ${rtld_impl} ${rtld_decl})
SOURCE_GROUP(rtld FILES ${rtld})
install( FILES ${rtld_decl} DESTINATION include/yack/rtld )

########################################################################
##
## string
##
########################################################################
file(GLOB string_impl yack/string/*.cpp)
file(GLOB string_decl yack/string/*.hpp)
set(string ${string_impl} ${string_decl} yack/string/string.hxx)
SOURCE_GROUP(string FILES ${string})
install( FILES ${string_decl} DESTINATION include/yack/string )

########################################################################
##
## math
##
########################################################################
file(GLOB math_impl yack/math/*.cpp)
file(GLOB math_decl yack/math/*.hpp)
set(math ${math_impl} ${math_decl})
SOURCE_GROUP(math FILES ${math})
install( FILES ${math_decl} DESTINATION include/yack/math )

########################################################################
##
## ptr
##
########################################################################
file(GLOB ptr_impl yack/ptr/*.cpp)
file(GLOB ptr_decl yack/ptr/*.hpp)
set(ptr ${ptr_impl} ${ptr_decl})
SOURCE_GROUP(ptr FILES ${ptr})
install( FILES ${ptr_decl} DESTINATION include/yack/ptr )



# create the library
add_library(yack
${src}
${type}
${arith}
${check}
${system}
${data} ${data_list} ${data_pool} ${data_small} ${data_suffix} ${data_hash}
${container} ${container_matrix} ${container_iterator}
${sequence}
${associative} ${associative_hash} ${associative_suffix}
${memory} ${memory_allocator} ${memory_small} ${memory_large} ${memory_joint} ${memory_buffer}
${fft}
${functor}
${apex} ${apex_m} ${apex_n} ${apex_z} ${apex_q}
${concurrent} ${concurrent_quark} ${concurrent_loop} ${concurrent_queue}
${ios} ${ios_c} ${ios_gz} ${ios_fmt} ${ios_gv} ${ios_ascii} ${ios_serializer}
${randomized}
${sort}
${counting} ${counting_c}
${rtld}
${string}
${kr}
${fs} ${fs_local}
${hashing}
${info} ${info_modulation}
${math}
${ptr}
${utest}
)

install( TARGETS yack ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(yack)

