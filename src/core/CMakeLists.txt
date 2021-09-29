project(core C CXX)

include_directories(.)

# top-level files
file(GLOB src_impl yack/*.c yack/*.cpp)
file(GLOB src_decl yack/*.h yack/*.hpp)
set(src ${src_impl} ${src_decl})
install( FILES ${src_decl} DESTINATION include/yack )

# unit tests structure
file(GLOB utest_impl yack/utest/*.cpp)
file(GLOB utest_decl yack/utest/*.hpp)
set(utest ${utest_impl} ${utest_decl})
SOURCE_GROUP(utest FILES ${utest})
install( FILES ${utest_decl} DESTINATION include/yack/utest )

# create the library
add_library(yack
${src}
${utest}
)

install( TARGETS yack ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(yack)

