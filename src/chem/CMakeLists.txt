include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB chem_main_decl yack/chem/*.hpp)
file(GLOB chem_main_srcs yack/chem/*.cpp)
set(chem_main ${chem_main_decl} ${chem_main_srcs})
SOURCE_GROUP(chem FILES ${chem_main})
install( FILES ${chem_main_decl} DESTINATION include/yack/chem )

#eq
file(GLOB chem_eq_decl yack/chem/eq/*.hpp yack/chem/eq/*.hxx)
file(GLOB chem_eq_srcs yack/chem/eq/*.cpp)
set(chem_eq ${chem_eq_decl} ${chem_eq_srcs})
SOURCE_GROUP(chem/eq FILES ${chem_eq})
install( FILES ${chem_eq_decl} DESTINATION include/yack/chem/eq )

#lua
file(GLOB chem_lua_decl yack/chem/lua/*.hpp)
file(GLOB chem_lua_srcs yack/chem/lua/*.cpp)
set(chem_lua ${chem_lua_decl} ${chem_lua_srcs})
SOURCE_GROUP(chem/lua FILES ${chem_lua})
install( FILES ${chem_lua_decl} DESTINATION include/yack/chem/lua )

#plexus
file(GLOB chem_plexus_decl yack/chem/plexus/*.hpp yack/chem/plexus/*.hxx)
file(GLOB chem_plexus_srcs yack/chem/plexus/*.cpp)
set(chem_plexus ${chem_plexus_decl} ${chem_plexus_srcs})
SOURCE_GROUP(chem/plexus FILES ${chem_plexus})
install( FILES ${chem_plexus_decl} DESTINATION include/yack/chem/plexus )

add_library(yack-chem STATIC
${chem_main}
${chem_eq}
${chem_plexus}
${chem_lua}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
