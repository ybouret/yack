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


add_library(yack-chem STATIC
${chem_main}
${chem_designer}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
