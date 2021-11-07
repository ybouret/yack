include_directories(. ../core)

file(GLOB math_main_decl yack/math/*.hpp)
file(GLOB math_main_srcs yack/math/*.cpp)
set(math_main ${math_main_decl} ${math_main_srcs})

SOURCE_GROUP(math FILES ${math_main})
install( FILES ${math_main_decl} DESTINATION include/yack/math )

file(GLOB math_algebra_decl yack/math/algebra/*.hpp)
file(GLOB math_algebra_srcs yack/math/algebra/*.cpp)
set(math_algebra ${math_algebra_decl} ${math_algebra_srcs})

SOURCE_GROUP(math/algebra FILES ${math_algebra})
install( FILES ${math_algebra_decl} DESTINATION include/yack/math/algebra )

add_library(yack-math STATIC
${math_main}
${math_algebra}
)

install( TARGETS yack-math ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(math)
