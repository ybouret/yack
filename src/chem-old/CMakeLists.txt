include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB chem_main_decl yack/chem/*.hpp)
file(GLOB chem_main_srcs yack/chem/*.cpp yack/chem/*.hxx)
set(chem_main ${chem_main_decl} ${chem_main_srcs})
SOURCE_GROUP(chem FILES ${chem_main})
install( FILES ${chem_main_decl} DESTINATION include/yack/chem )

#com
file(GLOB chem_com_decl yack/chem/com/*.hpp)
file(GLOB chem_com_srcs yack/chem/com/*.cpp yack/chem/com/*.hxx)
set(chem_com ${chem_com_decl} ${chem_com_srcs})
SOURCE_GROUP(chem/com FILES ${chem_com})
install( FILES ${chem_com_decl} DESTINATION include/yack/chem/com )

#weasel
file(GLOB chem_weasel_decl yack/chem/weasel/*.hpp)
file(GLOB chem_weasel_srcs yack/chem/weasel/*.cpp yack/chem/weasel/*.hxx)
set(chem_weasel ${chem_weasel_decl} ${chem_weasel_srcs})
SOURCE_GROUP(chem/weasel FILES ${chem_weasel})
install( FILES ${chem_weasel_decl} DESTINATION include/yack/chem/weasel )

#lua
file(GLOB chem_lua_decl yack/chem/lua/*.hpp)
file(GLOB chem_lua_srcs yack/chem/lua/*.cpp yack/chem/lua/*.hxx)
set(chem_lua ${chem_lua_decl} ${chem_lua_srcs})
SOURCE_GROUP(chem/lua FILES ${chem_lua})
install( FILES ${chem_lua_decl} DESTINATION include/yack/chem/lua )

#eqs
file(GLOB chem_eqs_decl yack/chem/eqs/*.hpp)
file(GLOB chem_eqs_srcs yack/chem/eqs/*.cpp yack/chem/eqs/*.hxx)
set(chem_eqs ${chem_eqs_decl} ${chem_eqs_srcs})
SOURCE_GROUP(chem/eqs FILES ${chem_eqs})
install( FILES ${chem_eqs_decl} DESTINATION include/yack/chem/eqs )

#realm
file(GLOB chem_realm_decl yack/chem/realm/*.hpp)
file(GLOB chem_realm_srcs yack/chem/realm/*.cpp)
set(chem_realm ${chem_realm_decl} ${chem_realm_srcs})
SOURCE_GROUP(chem/realm FILES ${chem_realm})
install( FILES ${chem_realm_decl} DESTINATION include/yack/chem/realm )

#realm/dom
file(GLOB chem_realm_dom_decl yack/chem/realm/dom/*.hpp)
file(GLOB chem_realm_dom_srcs yack/chem/realm/dom/*.cpp)
set(chem_realm_dom ${chem_realm_dom_decl} ${chem_realm_dom_srcs})
SOURCE_GROUP(chem/realm/dom FILES ${chem_realm_dom})
#install( FILES ${chem_realm_dom_decl} DESTINATION include/yack/chem/realm/dom )

#realm/claw
file(GLOB chem_realm_claw_decl yack/chem/realm/claw/*.hpp)
file(GLOB chem_realm_claw_srcs yack/chem/realm/claw/*.cpp)
set(chem_realm_claw ${chem_realm_claw_decl} ${chem_realm_claw_srcs})
SOURCE_GROUP(chem/realm/claw FILES ${chem_realm_claw})
install( FILES ${chem_realm_claw_decl} DESTINATION include/yack/chem/realm/claw )

#realm/coll
file(GLOB chem_realm_coll_decl yack/chem/realm/coll/*.hpp)
file(GLOB chem_realm_coll_srcs yack/chem/realm/coll/*.cpp)
set(chem_realm_coll ${chem_realm_coll_decl} ${chem_realm_coll_srcs})
SOURCE_GROUP(chem/realm/coll FILES ${chem_realm_coll})
install( FILES ${chem_realm_coll_decl} DESTINATION include/yack/chem/realm/coll )

add_library(yack-chem STATIC
${chem_main}
${chem_com}
${chem_eqs}
${chem_weasel}
${chem_lua}
${chem_realm} ${chem_realm_dom} ${chem_realm_claw} ${chem_realm_coll} 
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
yack_libs(yack-chem)
