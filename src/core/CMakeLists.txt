project(core C CXX)

include_directories(.)

# top-level files
file(GLOB src_impl yack/*.c yack/*.cpp)
file(GLOB src_decl yack/*.h yack/*.hpp)
set(src ${src_impl} ${src_decl})
install( FILES ${src_decl} DESTINATION include/yack )

# type operation
file(GLOB type_impl yack/type/*.cpp yack/type/*.c)
file(GLOB type_decl yack/type/*.hpp yack/type/*.h)
set(type ${type_impl} ${type_decl})
SOURCE_GROUP(type FILES ${type})
install( FILES ${type_decl} DESTINATION include/yack/type )

# arith operation
file(GLOB arith_impl yack/arith/*.cpp yack/arith/*.c)
file(GLOB arith_decl yack/arith/*.hpp yack/arith/*.h)
set(arith ${arith_impl} ${arith_decl})
SOURCE_GROUP(arith FILES ${arith})
install( FILES ${arith_decl} DESTINATION include/yack/arith )


# checking operations
file(GLOB check_impl yack/check/*.cpp yack/check/*.c)
file(GLOB check_decl yack/check/*.hpp yack/check/*.h)
set(check ${check_impl} ${check_decl})
SOURCE_GROUP(check FILES ${check})
install( FILES ${check_decl} DESTINATION include/yack/check )

# system operations
file(GLOB system_impl yack/system/*.cpp yack/system/*.c)
file(GLOB system_decl yack/system/*.hpp yack/system/*.h)
set(system ${system_impl} ${system_decl})
SOURCE_GROUP(system FILES ${system})
install( FILES ${system_decl} DESTINATION include/yack/system )

# unit tests structure
file(GLOB utest_impl yack/utest/*.cpp)
file(GLOB utest_decl yack/utest/*.hpp)
set(utest ${utest_impl} ${utest_decl})
SOURCE_GROUP(utest FILES ${utest})
install( FILES ${utest_decl} DESTINATION include/yack/utest )

# data structure operations
file(GLOB data_impl yack/data/*.cpp yack/data/*.c)
file(GLOB data_decl yack/data/*.hpp yack/data/*.h)
set(data ${data_impl} ${data_decl})
SOURCE_GROUP(data FILES ${data})
install( FILES ${data_decl} DESTINATION include/yack/data )

# container  operations
file(GLOB container_impl yack/container/*.cpp yack/container/*.c)
file(GLOB container_decl yack/container/*.hpp yack/container/*.h)
set(container ${container_impl} ${container_decl})
SOURCE_GROUP(container FILES ${container})
install( FILES ${container_decl} DESTINATION include/yack/container )

# memory operations
file(GLOB memory_impl yack/memory/*.cpp)
file(GLOB memory_decl yack/memory/*.hpp)
set(memory ${memory_impl} ${memory_decl})
SOURCE_GROUP(memory FILES ${memory})
install( FILES ${memory_decl} DESTINATION include/yack/memory )
 
 
# synchronic operations
file(GLOB synchronic_impl yack/synchronic/*.cpp)
file(GLOB synchronic_decl yack/synchronic/*.hpp)
set(synchronic ${synchronic_impl} ${synchronic_decl})
SOURCE_GROUP(synchronic FILES ${synchronic})
install( FILES ${synchronic_decl} DESTINATION include/yack/synchronic )
 
# synchronic/quark operations
file(GLOB synchronic_quark_impl yack/synchronic/quark/*.cpp)
file(GLOB synchronic_quark_decl yack/synchronic/quark/*.hpp)
set(synchronic_quark ${synchronic_quark_impl} ${synchronic_quark_decl})
SOURCE_GROUP(synchronic/quark FILES ${synchronic_quark})
install( FILES ${synchronic_quark_decl} DESTINATION include/yack/synchronic/quark )

# create the library
add_library(yack
${src}
${type}
${arith}
${check}
${system}
${data}
${container}
${memory}
${synchronic} ${synchronic_quark}
${utest}
)

install( TARGETS yack ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(yack)

