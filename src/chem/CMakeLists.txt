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

add_library(yack-chem STATIC
${chem_main}
${chem_com}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
yack_libs(yack-chem)
