include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB chem_main_decl yack/chem/*.hpp yack/chem/*.hxx)
file(GLOB chem_main_srcs yack/chem/*.cpp)
set(chem_main ${chem_main_decl} ${chem_main_srcs})
SOURCE_GROUP(chem FILES ${chem_main})
install( FILES ${chem_main_decl} DESTINATION include/yack/chem )

#eqs
file(GLOB chem_eqs_decl yack/chem/eqs/*.hpp yack/chem/eqs/*.hxx)
file(GLOB chem_eqs_srcs yack/chem/eqs/*.cpp)
set(chem_eqs ${chem_eqs_decl} ${chem_eqs_srcs})
SOURCE_GROUP(chem/eqs FILES ${chem_eqs})
install( FILES ${chem_eqs_decl} DESTINATION include/yack/chem/eqs )



#lua
file(GLOB chem_lua_decl yack/chem/lua/*.hpp yack/chem/lua/*.hxx)
file(GLOB chem_lua_srcs yack/chem/lua/*.cpp)
set(chem_lua ${chem_lua_decl} ${chem_lua_srcs})
SOURCE_GROUP(chem/lua FILES ${chem_lua})
install( FILES ${chem_lua_decl} DESTINATION include/yack/chem/lua )

#mix
file(GLOB chem_mix_decl yack/chem/mix/*.hpp yack/chem/mix/*.hxx)
file(GLOB chem_mix_srcs yack/chem/mix/*.cpp)
set(chem_mix ${chem_mix_decl} ${chem_mix_srcs})
SOURCE_GROUP(chem/mix FILES ${chem_mix})
install( FILES ${chem_mix_decl} DESTINATION include/yack/chem/mix )


#reactor
file(GLOB chem_reactor_decl yack/chem/reactor/*.hpp)
file(GLOB chem_reactor_srcs yack/chem/reactor/*.cpp)
set(chem_reactor ${chem_reactor_decl} ${chem_reactor_srcs})
SOURCE_GROUP(chem/reactor FILES ${chem_reactor})
install( FILES ${chem_reactor_decl} DESTINATION include/yack/chem/reactor )

add_library(yack-chem STATIC
${chem_main}
${chem_eqs}
${chem_lua}
${chem_mix}
${chem_reactor}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
