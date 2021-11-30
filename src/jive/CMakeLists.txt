include_directories(. ../core)

#main
file(GLOB jive_main_decl yack/jive/*.hpp)
file(GLOB jive_main_srcs yack/jive/*.cpp)
set(jive_main ${jive_main_decl} ${jive_main_srcs})
SOURCE_GROUP(jive FILES ${jive_main})
install( FILES ${jive_main_decl} DESTINATION include/yack/jive )

add_library(yack-jive STATIC
${jive_main}
)

install( TARGETS yack-jive ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(jive)
