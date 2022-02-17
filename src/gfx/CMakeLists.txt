include_directories(. ../core ../jive)

#main
file(GLOB gfx_main_decl yack/gfx/*.hpp)
file(GLOB gfx_main_srcs yack/gfx/*.cpp)
set(gfx_main ${gfx_main_decl} ${gfx_main_srcs})
SOURCE_GROUP(gfx FILES ${gfx_main})
install( FILES ${gfx_main_decl} DESTINATION include/yack/gfx )


add_library(yack-gfx STATIC
${gfx_main}
)

install( TARGETS yack-gfx ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(gfx)
