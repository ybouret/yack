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

#nexus
file(GLOB chem_nexus_decl yack/chem/nexus/*.hpp)
file(GLOB chem_nexus_srcs yack/chem/nexus/*.cpp)
set(chem_nexus ${chem_nexus_decl} ${chem_nexus_srcs})
SOURCE_GROUP(chem/nexus FILES ${chem_nexus})
#install( FILES ${chem_nexus_decl} DESTINATION include/yack/chem/nexus )

add_library(yack-chem STATIC
${chem_main}
${chem_designer}
${chem_eqs}
${chem_nexus}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
yack_libs(yack-chem)

