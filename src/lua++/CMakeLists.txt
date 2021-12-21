include_directories(. ../core)

#main
file(GLOB lua_main_decl yack/lua++/*.hpp)
file(GLOB lua_main_srcs yack/lua++/*.cpp)
set(lua_main ${lua_main_decl} ${lua_main_srcs})
SOURCE_GROUP(lua++ FILES ${lua_main})
install( FILES ${lua_main_decl} DESTINATION include/yack/lua++ )
 

add_library(yack-lua++ STATIC
${lua_main}
)

install( TARGETS yack-lua++ ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(lua++)
