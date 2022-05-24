message( STATUS "build: ${CMAKE_CURRENT_BINARY_DIR}")
include_directories(. ../core ../jive)
include_directories( ../yack/png )
include_directories( ../yack/jpeg)
include_directories( ../yack/tiff/libtiff ${CMAKE_CURRENT_BINARY_DIR}/../yack/tiff/libtiff)

#main
file(GLOB gfx_main_decl yack/gfx/*.hpp)
file(GLOB gfx_main_srcs yack/gfx/*.cpp)
set(gfx_main ${gfx_main_decl} ${gfx_main_srcs})
SOURCE_GROUP(gfx FILES ${gfx_main})
install( FILES ${gfx_main_decl} DESTINATION include/yack/gfx )

#image
file(GLOB gfx_image_decl yack/gfx/image/*.hpp)
file(GLOB gfx_image_srcs yack/gfx/image/*.cpp)
set(gfx_image ${gfx_image_decl} ${gfx_image_srcs})
SOURCE_GROUP(gfx/image FILES ${gfx_image})
install( FILES ${gfx_image_decl} DESTINATION include/yack/gfx/image )


#image/format
file(GLOB gfx_image_format_decl yack/gfx/image/format/*.hpp)
file(GLOB gfx_image_format_srcs yack/gfx/image/format/*.cpp)
set(gfx_image_format ${gfx_image_format_decl} ${gfx_image_format_srcs})
SOURCE_GROUP(gfx/image/format FILES ${gfx_image_format})
install( FILES ${gfx_image_format_decl} DESTINATION include/yack/gfx/image/format )

#filter
file(GLOB gfx_filter_decl yack/gfx/filter/*.hpp)
file(GLOB gfx_filter_srcs yack/gfx/filter/*.cpp)
set(gfx_filter ${gfx_filter_decl} ${gfx_filter_srcs})
SOURCE_GROUP(gfx/filter FILES ${gfx_filter})
install( FILES ${gfx_filter_decl} DESTINATION include/yack/gfx/filter )

#pixel
file(GLOB gfx_pixel_decl yack/gfx/pixel/*.hpp)
file(GLOB gfx_pixel_srcs yack/gfx/pixel/*.cpp)
set(gfx_pixel ${gfx_pixel_decl} ${gfx_pixel_srcs})
SOURCE_GROUP(gfx/pixel FILES ${gfx_pixel})
install( FILES ${gfx_pixel_decl} DESTINATION include/yack/gfx/pixel )


#broker
file(GLOB gfx_broker_decl yack/gfx/broker/*.hpp)
file(GLOB gfx_broker_srcs yack/gfx/broker/*.cpp)
set(gfx_broker ${gfx_broker_decl} ${gfx_broker_srcs})
SOURCE_GROUP(gfx/broker FILES ${gfx_broker})
install( FILES ${gfx_broker_decl} DESTINATION include/yack/gfx/broker )

#edges
file(GLOB gfx_edges_decl yack/gfx/edges/*.hpp)
file(GLOB gfx_edges_srcs yack/gfx/edges/*.cpp)
set(gfx_edges ${gfx_edges_decl} ${gfx_edges_srcs})
SOURCE_GROUP(gfx/edges FILES ${gfx_edges})
install( FILES ${gfx_edges_decl} DESTINATION include/yack/gfx/edges )



add_library(yack-gfx STATIC
${gfx_main}
${gfx_image}
${gfx_image_format}
${gfx_filter}
${gfx_pixel}
${gfx_broker}
${gfx_edges}
)

install( TARGETS yack-gfx ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(gfx)
