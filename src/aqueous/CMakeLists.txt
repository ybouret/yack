include_directories(. ../core ../jive ../math ../lua++ ..)

#main
file(GLOB aqueous_main_decl yack/aqueous/*.hpp)
file(GLOB aqueous_main_srcs yack/aqueous/*.cpp yack/aqueous/*.hxx)
set(aqueous_main ${aqueous_main_decl} ${aqueous_main_srcs})
SOURCE_GROUP(aqueous FILES ${aqueous_main})
install( FILES ${aqueous_main_decl} DESTINATION include/yack/aqueous )

#weasel
file(GLOB aqueous_weasel_decl yack/aqueous/weasel/*.hpp)
file(GLOB aqueous_weasel_srcs yack/aqueous/weasel/*.cpp yack/aqueous/weasel/*.hxx)
set(aqueous_weasel ${aqueous_weasel_decl} ${aqueous_weasel_srcs})
SOURCE_GROUP(aqueous/weasel FILES ${aqueous_weasel})
install( FILES ${aqueous_weasel_decl} DESTINATION include/yack/aqueous/weasel )


add_library(yack-aqueous STATIC
${aqueous_main}
${aqueous_weasel}
)

install( TARGETS yack-aqueous ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(aqueous)
yack_libs(yack-aqueous)
