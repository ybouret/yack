include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB aqueous_main_decl yack/aqueous/*.hpp)
file(GLOB aqueous_main_srcs yack/aqueous/*.cpp yack/aqueous/*.hxx)
set(aqueous_main ${aqueous_main_decl} ${aqueous_main_srcs})
SOURCE_GROUP(aqueous FILES ${aqueous_main})
install( FILES ${aqueous_main_decl} DESTINATION include/yack/aqueous )

#weasel
file(GLOB aqueous_weasel_decl yack/aqueous/weasel/*.hpp)
file(GLOB aqueous_weasel_srcs yack/aqueous/weasel/*.cpp yack/aqueous/weasel/*.hxx)
set(aqueous_weasel ${aqueous_weasel_decl} ${aqueous_weasel_srcs})
SOURCE_GROUP(aqueous/weasel FILES ${aqueous_weasel})
install( FILES ${aqueous_weasel_decl} DESTINATION include/yack/aqueous/weasel )

#lua
file(GLOB aqueous_lua_decl yack/aqueous/lua/*.hpp)
file(GLOB aqueous_lua_srcs yack/aqueous/lua/*.cpp yack/aqueous/lua/*.hxx)
set(aqueous_lua ${aqueous_lua_decl} ${aqueous_lua_srcs})
SOURCE_GROUP(aqueous/lua FILES ${aqueous_lua})
install( FILES ${aqueous_lua_decl} DESTINATION include/yack/aqueous/lua )

add_library(yack-aqueous STATIC
${aqueous_main}
${aqueous_weasel}
${aqueous_lua}
)

install( TARGETS yack-aqueous ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(aqueous)
yack_libs(yack-aqueous)
