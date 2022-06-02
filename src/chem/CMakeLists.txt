include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB chem_main_decl yack/chem/*.hpp yack/chem/*.hxx)
file(GLOB chem_main_srcs yack/chem/*.cpp)
set(chem_main ${chem_main_decl} ${chem_main_srcs})
SOURCE_GROUP(chem FILES ${chem_main})
install( FILES ${chem_main_decl} DESTINATION include/yack/chem )

#lua
file(GLOB chem_lua_decl yack/chem/lua/*.hpp yack/chem/lua/*.hxx)
file(GLOB chem_lua_srcs yack/chem/lua/*.cpp)
set(chem_lua ${chem_lua_decl} ${chem_lua_srcs})
SOURCE_GROUP(chem/lua FILES ${chem_lua})
install( FILES ${chem_lua_decl} DESTINATION include/yack/chem/lua )


#reactor
file(GLOB chem_reactor_decl yack/chem/reactor/*.hpp)
file(GLOB chem_reactor_srcs yack/chem/reactor/*.cpp)
set(chem_reactor ${chem_reactor_decl} ${chem_reactor_srcs})
SOURCE_GROUP(chem/reactor FILES ${chem_reactor})
install( FILES ${chem_reactor_decl} DESTINATION include/yack/chem/reactor )

 
add_library(yack-chem STATIC
${chem_main}
${chem_lua}
${chem_reactor}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
