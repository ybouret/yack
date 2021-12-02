include_directories(. ../core)

#main
file(GLOB jive_main_decl yack/jive/*.hpp)
file(GLOB jive_main_srcs yack/jive/*.cpp)
set(jive_main ${jive_main_decl} ${jive_main_srcs})
SOURCE_GROUP(jive FILES ${jive_main})
install( FILES ${jive_main_decl} DESTINATION include/yack/jive )

#pattern
file(GLOB jive_pattern_decl yack/jive/pattern/*.hpp)
file(GLOB jive_pattern_srcs yack/jive/pattern/*.cpp)
set(jive_pattern ${jive_pattern_decl} ${jive_pattern_srcs})
SOURCE_GROUP(jive/pattern FILES ${jive_pattern})
install( FILES ${jive_pattern_decl} DESTINATION include/yack/jive/pattern )

#pattern/basic
file(GLOB jive_pattern_basic_decl yack/jive/pattern/basic/*.hpp)
file(GLOB jive_pattern_basic_srcs yack/jive/pattern/basic/*.cpp)
set(jive_pattern_basic ${jive_pattern_basic_decl} ${jive_pattern_basic_srcs})
SOURCE_GROUP(jive/pattern/basic FILES ${jive_pattern_basic})
install( FILES ${jive_pattern_basic_decl} DESTINATION include/yack/jive/pattern/basic )


add_library(yack-jive STATIC
${jive_main}
${jive_pattern} ${jive_pattern_basic}
)

install( TARGETS yack-jive ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(jive)
