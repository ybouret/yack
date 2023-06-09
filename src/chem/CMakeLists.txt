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

#eqs
file(GLOB chem_eqs_decl yack/chem/eqs/*.hpp)
file(GLOB chem_eqs_srcs yack/chem/eqs/*.cpp yack/chem/eqs/*.hxx)
set(chem_eqs ${chem_eqs_decl} ${chem_eqs_srcs})
SOURCE_GROUP(chem/eqs FILES ${chem_eqs})
install( FILES ${chem_eqs_decl} DESTINATION include/yack/chem/eqs )

#weasel
file(GLOB chem_weasel_decl yack/chem/weasel/*.hpp)
file(GLOB chem_weasel_srcs yack/chem/weasel/*.cpp yack/chem/weasel/*.hxx)
set(chem_weasel ${chem_weasel_decl} ${chem_weasel_srcs})
SOURCE_GROUP(chem/weasel FILES ${chem_weasel})
install( FILES ${chem_weasel_decl} DESTINATION include/yack/chem/weasel )

#vat
file(GLOB chem_vat_decl yack/chem/vat/*.hpp)
file(GLOB chem_vat_srcs yack/chem/vat/*.cpp yack/chem/vat/*.hxx)
set(chem_vat ${chem_vat_decl} ${chem_vat_srcs})
SOURCE_GROUP(chem/vat FILES ${chem_vat})
install( FILES ${chem_vat_decl} DESTINATION include/yack/chem/vat )

#vat/cluster
file(GLOB chem_vat_cluster_decl yack/chem/vat/cluster/*.hpp)
file(GLOB chem_vat_cluster_srcs yack/chem/vat/cluster/*.cpp)
set(chem_vat_cluster ${chem_vat_cluster_decl} ${chem_vat_cluster_srcs})
SOURCE_GROUP(chem/vat/cluster FILES ${chem_vat_cluster})
install( FILES ${chem_vat_cluster_decl} DESTINATION include/yack/chem/vat/cluster )

#vat/equalizer
file(GLOB chem_vat_equalizer_decl yack/chem/vat/equalizer/*.hpp)
file(GLOB chem_vat_equalizer_srcs yack/chem/vat/equalizer/*.cpp)
set(chem_vat_equalizer ${chem_vat_equalizer_decl} ${chem_vat_equalizer_srcs})
SOURCE_GROUP(chem/vat/equalizer FILES ${chem_vat_equalizer})
install( FILES ${chem_vat_equalizer_decl} DESTINATION include/yack/chem/vat/equalizer )

add_library(yack-chem STATIC
${chem_main}
${chem_com}
${chem_eqs}
${chem_weasel}
${chem_vat} ${chem_vat_cluster} ${chem_vat_equalizer}
)

install( TARGETS yack-chem ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(chem)
yack_libs(yack-chem)
