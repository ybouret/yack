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


# memory operations
file(GLOB memory_impl yack/memory/*.cpp yack/memory/*.c)
file(GLOB memory_decl yack/memory/*.hpp yack/memory/*.h)
set(memory ${memory_impl} ${memory_decl})
SOURCE_GROUP(memory FILES ${memory})
install( FILES ${memory_decl} DESTINATION include/yack/memory )
 

# create the library
add_library(yack
${src}
${type}
${arith}
${check}
${system}
${memory}
${utest}
)

install( TARGETS yack ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(yack)

