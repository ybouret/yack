include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB aqueous_main_decl yack/aqueous/*.hpp)
file(GLOB aqueous_main_srcs yack/aqueous/*.cpp yack/aqueous/*.hxx)
set(aqueous_main ${aqueous_main_decl} ${aqueous_main_srcs})
SOURCE_GROUP(aqueous FILES ${aqueous_main})
install( FILES ${aqueous_main_decl} DESTINATION include/yack/aqueous )

add_library(yack-aqueous STATIC
${aqueous_main}
)

install( TARGETS yack-aqueous ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(aqueous)
yack_libs(yack-aqueous)
