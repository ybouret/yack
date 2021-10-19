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

########################################################################
##
## container  operations
##
########################################################################
file(GLOB container_impl yack/container/*.cpp yack/container/*.c)
file(GLOB container_decl yack/container/*.hpp yack/container/*.h)
set(container ${container_impl} ${container_decl})
SOURCE_GROUP(container FILES ${container})
install( FILES ${container_decl} DESTINATION include/yack/container )

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
 
file(GLOB apex_n_impl yack/apex/n/*.cpp)
file(GLOB apex_n_decl yack/apex/n/*.hpp)
set(apex_n ${apex_n_impl} ${apex_n_decl})
SOURCE_GROUP(apex/n FILES ${apex_n})
install( FILES ${apex_n_decl} DESTINATION include/yack/apex/n ) 

file(GLOB apex_m_impl yack/apex/m/*.cpp)
file(GLOB apex_m_decl yack/apex/m/*.hpp)
set(apex_m ${apex_m_impl} ${apex_m_decl})
SOURCE_GROUP(apex/m FILES ${apex_m})
install( FILES ${apex_m_decl} DESTINATION include/yack/apex/m ) 

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


# create the library
add_library(yack
${src}
${type}
${arith}
${check}
${system}
${data}
${container}
${memory} ${memory_allocator} ${memory_small} ${memory_large} ${memory_joint}
${fft}
${apex} ${apex_n} ${apex_m}
${concurrent} ${concurrent_quark} ${concurrent_loop}
${utest}
)

install( TARGETS yack ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(yack)

