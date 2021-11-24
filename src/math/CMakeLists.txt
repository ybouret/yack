include_directories(. ../core)

#main
file(GLOB math_main_decl yack/math/*.hpp)
file(GLOB math_main_srcs yack/math/*.cpp)
set(math_main ${math_main_decl} ${math_main_srcs})
SOURCE_GROUP(math FILES ${math_main})
install( FILES ${math_main_decl} DESTINATION include/yack/math )

# tao
file(GLOB math_tao_decl yack/math/tao/*.hpp)
file(GLOB math_tao_srcs yack/math/tao/*.cpp)
set(math_tao ${math_tao_decl} ${math_tao_srcs})
SOURCE_GROUP(math/tao FILES ${math_tao})
install( FILES ${math_tao_decl} DESTINATION include/yack/math/tao )

# algebra
file(GLOB math_algebra_decl yack/math/algebra/*.hpp)
file(GLOB math_algebra_srcs yack/math/algebra/*.cpp)
set(math_algebra ${math_algebra_decl} ${math_algebra_srcs})
SOURCE_GROUP(math/algebra FILES ${math_algebra})
install( FILES ${math_algebra_decl} DESTINATION include/yack/math/algebra )

#ode
file(GLOB math_ode_decl yack/math/ode/*.hpp)
file(GLOB math_ode_srcs yack/math/ode/*.cpp)
set(math_ode ${math_ode_decl} ${math_ode_srcs})
SOURCE_GROUP(math/ode FILES ${math_ode})
install( FILES ${math_ode_decl} DESTINATION include/yack/math/ode )

file(GLOB math_ode_explicit_decl yack/math/ode/explicit/*.hpp)
file(GLOB math_ode_explicit_srcs yack/math/ode/explicit/*.cpp yack/math/ode/explicit/*.hxx)
set(math_ode_explicit ${math_ode_explicit_decl} ${math_ode_explicit_srcs})
SOURCE_GROUP(math/ode/explicit FILES ${math_ode_explicit})
install( FILES ${math_ode_explicit_decl} DESTINATION include/yack/math/ode/explicit )

add_library(yack-math STATIC
${math_main}
${math_tao}
${math_algebra}
${math_ode} ${math_ode_explicit}
)

install( TARGETS yack-math ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(math)
