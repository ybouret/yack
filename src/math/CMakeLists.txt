include_directories(. ../core)

#main
file(GLOB math_main_decl yack/math/*.hpp)
file(GLOB math_main_srcs yack/math/*.cpp)
set(math_main ${math_main_decl} ${math_main_srcs})
SOURCE_GROUP(math FILES ${math_main})
install( FILES ${math_main_decl} DESTINATION include/yack/math )

# fcn
file(GLOB math_fcn_decl yack/math/fcn/*.hpp)
file(GLOB math_fcn_srcs yack/math/fcn/*.cpp yack/math/fcn/*.hxx)
set(math_fcn ${math_fcn_decl} ${math_fcn_srcs})
SOURCE_GROUP(math/fcn FILES ${math_fcn})
install( FILES ${math_fcn_decl} DESTINATION include/yack/math/fcn )


# tao
file(GLOB math_tao_decl yack/math/tao/*.hpp)
file(GLOB math_tao_srcs yack/math/tao/*.cpp)
set(math_tao ${math_tao_decl} ${math_tao_srcs})
SOURCE_GROUP(math/tao FILES ${math_tao})
install( FILES ${math_tao_decl} DESTINATION include/yack/math/tao )

# algebra
file(GLOB math_algebra_decl yack/math/algebra/*.hpp)
file(GLOB math_algebra_srcs yack/math/algebra/*.cpp yack/math/algebra/*.hxx)
set(math_algebra ${math_algebra_decl} ${math_algebra_srcs})
SOURCE_GROUP(math/algebra FILES ${math_algebra})
install( FILES ${math_algebra_decl} DESTINATION include/yack/math/algebra )

# root
file(GLOB math_root_decl yack/math/root/*.hpp)
file(GLOB math_root_srcs yack/math/root/*.cpp yack/math/root/*.hxx)
set(math_root ${math_root_decl} ${math_root_srcs})
SOURCE_GROUP(math/root FILES ${math_root})
install( FILES ${math_root_decl} DESTINATION include/yack/math/root )

# opt
file(GLOB math_opt_decl yack/math/opt/*.hpp)
file(GLOB math_opt_srcs yack/math/opt/*.cpp)
set(math_opt ${math_opt_decl} ${math_opt_srcs})
SOURCE_GROUP(math/opt FILES ${math_opt})
install( FILES ${math_opt_decl} DESTINATION include/yack/math/opt )

# data
file(GLOB math_data_decl yack/math/data/*.hpp)
file(GLOB math_data_srcs yack/math/data/*.cpp yack/math/data/*.hxx)
set(math_data ${math_data_decl} ${math_data_srcs})
SOURCE_GROUP(math/data FILES ${math_data})
install( FILES ${math_data_decl} DESTINATION include/yack/math/data )

#ode
file(GLOB math_ode_decl yack/math/ode/*.hpp yack/math/ode/*.hxx)
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
${math_fcn}
${math_tao}
${math_algebra}
${math_ode} ${math_ode_explicit}
${math_root}
${math_opt}
${math_data}
)

install( TARGETS yack-math ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(math)
