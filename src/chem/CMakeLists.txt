include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB chem_main_decl yack/chem/*.hpp)
file(GLOB chem_main_srcs yack/chem/*.cpp yack/chem/*.hxx)
set(chem_main ${chem_main_decl} ${chem_main_srcs})
SOURCE_GROUP(chem FILES ${chem_main})
install( FILES ${chem_main_decl} DESTINATION include/yack/chem )

#designer
file(GLOB chem_designer_decl yack/chem/designer/*.hpp)
file(GLOB chem_designer_srcs yack/chem/designer/*.cpp)
set(chem_designer ${chem_designer_decl} ${chem_designer_srcs})
SOURCE_GROUP(chem/designer FILES ${chem_designer})
#install( FILES ${chem_designer_decl} DESTINATION include/yack/chem/designer )

#eqs
file(GLOB chem_eqs_decl yack/chem/eqs/*.hpp)
file(GLOB chem_eqs_srcs yack/chem/eqs/*.cpp)
set(chem_eqs ${chem_eqs_decl} ${chem_eqs_srcs})
SOURCE_GROUP(chem/eqs FILES ${chem_eqs})
install( FILES ${chem_eqs_decl} DESTINATION include/yack/chem/eqs )

#reactor
file(GLOB chem_reactor_decl yack/chem/reactor/*.hpp)
file(GLOB chem_reactor_srcs yack/chem/reactor/*.cpp)
set(chem_reactor ${chem_reactor_decl} ${chem_reactor_srcs})
SOURCE_GROUP(chem/reactor FILES ${chem_reactor})
install( FILES ${chem_reactor_decl} DESTINATION include/yack/chem/reactor )

#reactor/conservation
file(GLOB chem_reactor_conservation_decl yack/chem/reactor/conservation/*.hpp)
file(GLOB chem_reactor_conservation_srcs yack/chem/reactor/conservation/*.cpp)
set(chem_reactor_conservation ${chem_reactor_conservation_decl} ${chem_reactor_conservation_srcs})
SOURCE_GROUP(chem/reactor/conservation FILES ${chem_reactor_conservation})
install( FILES ${chem_reactor_conservation_decl} DESTINATION include/yack/chem/reactor/conservation )

#reactor/equilibrium
file(GLOB chem_reactor_equilibrium_decl yack/chem/reactor/equilibrium/*.hpp)
file(GLOB chem_reactor_equilibrium_srcs yack/chem/reactor/equilibrium/*.cpp)
set(chem_reactor_equilibrium ${chem_reactor_equilibrium_decl} ${chem_reactor_equilibrium_srcs})
SOURCE_GROUP(chem/reactor/equilibrium FILES ${chem_reactor_equilibrium})
install( FILES ${chem_reactor_equilibrium_decl} DESTINATION include/yack/chem/reactor/equilibrium )

#reactor/species
file(GLOB chem_reactor_species_decl yack/chem/reactor/species/*.hpp)
file(GLOB chem_reactor_species_srcs yack/chem/reactor/species/*.cpp)
set(chem_reactor_species ${chem_reactor_species_decl} ${chem_reactor_species_srcs})
SOURCE_GROUP(chem/reactor/species FILES ${chem_reactor_species})
install( FILES ${chem_reactor_species_decl} DESTINATION include/yack/chem/reactor/species )

add_library(yack-chem STATIC
${chem_main}
${chem_designer}
${chem_eqs}
${chem_reactor} ${chem_reactor_conservation} ${chem_reactor_equilibrium} ${chem_reactor_species} 
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
yack_libs(yack-chem)

