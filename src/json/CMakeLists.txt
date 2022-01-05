include_directories(. ../core ../jive)

#main
file(GLOB json_main_decl yack/json/*.hpp)
file(GLOB json_main_srcs yack/json/*.cpp)
set(json_main ${json_main_decl} ${json_main_srcs})
SOURCE_GROUP(json FILES ${json_main})
install( FILES ${json_main_decl} DESTINATION include/yack/json )


add_library(yack-json STATIC
${json_main}
)

install( TARGETS yack-json ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(json)
