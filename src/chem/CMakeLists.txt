include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB chem_main_decl yack/chem/*.hpp)
file(GLOB chem_main_srcs yack/chem/*.cpp)
set(chem_main ${chem_main_decl} ${chem_main_srcs})
SOURCE_GROUP(chem FILES ${chem_main})
install( FILES ${chem_main_decl} DESTINATION include/yack/chem )

##eq
#file(GLOB chem_eq_decl yack/chem/eq/*.hpp yack/chem/eq/*.hxx)
#file(GLOB chem_eq_srcs yack/chem/eq/*.cpp)
#set(chem_eq ${chem_eq_decl} ${chem_eq_srcs})
#SOURCE_GROUP(chem/eq FILES ${chem_eq})
#install( FILES ${chem_eq_decl} DESTINATION include/yack/chem/eq )

 
add_library(yack-chem STATIC
${chem_main}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
