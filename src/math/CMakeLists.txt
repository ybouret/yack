include_directories(. ../core)

#main
file(GLOB math_main_decl yack/math/*.hpp)
file(GLOB math_main_srcs yack/math/*.cpp yack/math/*.hxx)
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
file(GLOB math_opt_srcs yack/math/opt/*.cpp yack/math/opt/*.hxx)
set(math_opt ${math_opt_decl} ${math_opt_srcs})
SOURCE_GROUP(math/opt FILES ${math_opt})
install( FILES ${math_opt_decl} DESTINATION include/yack/math/opt )

# data
file(GLOB math_data_decl yack/math/data/*.hpp)
file(GLOB math_data_srcs yack/math/data/*.cpp yack/math/data/*.hxx)
set(math_data ${math_data_decl} ${math_data_srcs})
SOURCE_GROUP(math/data FILES ${math_data})
install( FILES ${math_data_decl} DESTINATION include/yack/math/data )

# fit
file(GLOB math_fit_decl yack/math/fit/*.hpp)
file(GLOB math_fit_srcs yack/math/fit/*.cpp yack/math/fit/*.hxx)
set(math_fit ${math_fit_decl} ${math_fit_srcs})
SOURCE_GROUP(math/fit FILES ${math_fit})
install( FILES ${math_fit_decl} DESTINATION include/yack/math/fit )

# fit/variable
file(GLOB math_fit_variable_decl yack/math/fit/variable/*.hpp)
file(GLOB math_fit_variable_srcs yack/math/fit/variable/*.cpp yack/math/fit/variable/*.hxx)
set(math_fit_variable ${math_fit_variable_decl} ${math_fit_variable_srcs})
SOURCE_GROUP(math/fit/variable FILES ${math_fit_variable})
install( FILES ${math_fit_variable_decl} DESTINATION include/yack/math/fit/variable )

# fit/sample
file(GLOB math_fit_sample_decl yack/math/fit/sample/*.hpp)
file(GLOB math_fit_sample_srcs yack/math/fit/sample/*.cpp yack/math/fit/sample/*.hxx)
set(math_fit_sample ${math_fit_sample_decl} ${math_fit_sample_srcs})
SOURCE_GROUP(math/fit/sample FILES ${math_fit_sample})
install( FILES ${math_fit_sample_decl} DESTINATION include/yack/math/fit/sample )

# fit/samples
file(GLOB math_fit_samples_decl yack/math/fit/samples/*.hpp)
file(GLOB math_fit_samples_srcs yack/math/fit/samples/*.cpp yack/math/fit/samples/*.hxx)
set(math_fit_samples ${math_fit_samples_decl} ${math_fit_samples_srcs})
SOURCE_GROUP(math/fit/samples FILES ${math_fit_samples})
install( FILES ${math_fit_samples_decl} DESTINATION include/yack/math/fit/samples )

# fit/least_squares
file(GLOB math_fit_least_squares_decl yack/math/fit/least-squares/*.hpp)
file(GLOB math_fit_least_squares_srcs yack/math/fit/least-squares/*.cpp yack/math/fit/least-squares/*.hxx)
set(math_fit_least_squares ${math_fit_least_squares_decl} ${math_fit_least_squares_srcs})
SOURCE_GROUP(math/fit/least-squares FILES ${math_fit_least_squares})
install( FILES ${math_fit_least_squares_decl} DESTINATION include/yack/math/fit/least-squares )

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

file(GLOB math_ode_explicit_rk45_decl yack/math/ode/explicit/rk45/*.hpp)
file(GLOB math_ode_explicit_rk45_srcs yack/math/ode/explicit/rk45/*.cpp yack/math/ode/explicit/rk45/*.hxx)
set(math_ode_explicit_rk45 ${math_ode_explicit_rk45_decl} ${math_ode_explicit_rk45_srcs})
SOURCE_GROUP(math/ode/explicit/rk45 FILES ${math_ode_explicit_rk45})
install( FILES ${math_ode_explicit_rk45_decl} DESTINATION include/yack/math/ode/explicit/rk45 )

file(GLOB math_ode_explicit_bs_decl yack/math/ode/explicit/bs/*.hpp)
file(GLOB math_ode_explicit_bs_srcs yack/math/ode/explicit/bs/*.cpp yack/math/ode/explicit/bs/*.hxx)
set(math_ode_explicit_bs ${math_ode_explicit_bs_decl} ${math_ode_explicit_bs_srcs})
SOURCE_GROUP(math/ode/explicit/bs FILES ${math_ode_explicit_bs})
install( FILES ${math_ode_explicit_bs_decl} DESTINATION include/yack/math/ode/explicit/bs )

add_library(yack-math STATIC
${math_main}
${math_fcn}
${math_tao}
${math_algebra}
${math_ode} ${math_ode_explicit} ${math_ode_explicit_rk45} ${math_ode_explicit_bs}
${math_root}
${math_opt}
${math_data}
${math_fit} ${math_fit_variable} ${math_fit_sample} ${math_fit_samples} ${math_fit_least_squares}
)

install( TARGETS yack-math ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(math)
