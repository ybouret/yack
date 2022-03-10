include_directories(. ../core ../jive ../yack/png)

#main
file(GLOB gfx_main_decl yack/gfx/*.hpp)
file(GLOB gfx_main_srcs yack/gfx/*.cpp)
set(gfx_main ${gfx_main_decl} ${gfx_main_srcs})
SOURCE_GROUP(gfx FILES ${gfx_main})
install( FILES ${gfx_main_decl} DESTINATION include/yack/gfx )

#format
file(GLOB gfx_format_decl yack/gfx/format/*.hpp)
file(GLOB gfx_format_srcs yack/gfx/format/*.cpp)
set(gfx_format ${gfx_format_decl} ${gfx_format_srcs})
SOURCE_GROUP(gfx/format FILES ${gfx_format})
install( FILES ${gfx_format_decl} DESTINATION include/yack/gfx/format )

add_library(yack-gfx STATIC
${gfx_main}
${gfx_format}
)

install( TARGETS yack-gfx ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(gfx)
