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

#sp
file(GLOB chem_sp_decl yack/chem/sp/*.hpp)
file(GLOB chem_sp_srcs yack/chem/sp/*.cpp)
set(chem_sp ${chem_sp_decl} ${chem_sp_srcs})
SOURCE_GROUP(chem/sp FILES ${chem_sp})
install( FILES ${chem_sp_decl} DESTINATION include/yack/chem/sp )

#eqs
file(GLOB chem_eqs_decl yack/chem/eqs/*.hpp)
file(GLOB chem_eqs_srcs yack/chem/eqs/*.cpp)
set(chem_eqs ${chem_eqs_decl} ${chem_eqs_srcs})
SOURCE_GROUP(chem/eqs FILES ${chem_eqs})
install( FILES ${chem_eqs_decl} DESTINATION include/yack/chem/eqs )

#claw
file(GLOB chem_claw_decl yack/chem/claw/*.hpp)
file(GLOB chem_claw_srcs yack/chem/claw/*.cpp)
set(chem_claw ${chem_claw_decl} ${chem_claw_srcs})
SOURCE_GROUP(chem/claw FILES ${chem_claw})
install( FILES ${chem_claw_decl} DESTINATION include/yack/chem/claw )

#nexus
file(GLOB chem_nexus_decl yack/chem/nexus/*.hpp)
file(GLOB chem_nexus_srcs yack/chem/nexus/*.cpp)
set(chem_nexus ${chem_nexus_decl} ${chem_nexus_srcs})
SOURCE_GROUP(chem/nexus FILES ${chem_nexus})
#install( FILES ${chem_nexus_decl} DESTINATION include/yack/chem/nexus )

#balancing
file(GLOB chem_balancing_decl yack/chem/balancing/*.hpp)
file(GLOB chem_balancing_srcs yack/chem/balancing/*.cpp)
set(chem_balancing ${chem_balancing_decl} ${chem_balancing_srcs})
SOURCE_GROUP(chem/balancing FILES ${chem_balancing})
install( FILES ${chem_balancing_decl} DESTINATION include/yack/chem/balancing )

add_library(yack-chem STATIC
${chem_main}
${chem_designer}
${chem_sp}
${chem_eqs}
${chem_nexus}
${chem_claw}
${chem_balancing}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
yack_libs(yack-chem)

