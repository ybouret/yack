project(core C CXX)

include_directories(.)

file(GLOB src_impl yack/*.c yack/*.cpp)
file(GLOB src_decl yack/*.h yack/*.hpp)
set(src ${src_impl} ${src_decl})

file(GLOB utest_impl yack/utest/*.cpp)
file(GLOB utest_decl yack/utest/*.hpp)
set(utest ${utest_impl} ${utest_decl})
SOURCE_GROUP(utest FILES ${utest})


add_library(yack
${src}
${utest}
)

add_subdirectory(tests)

