project(png C)
include_directories(. ../zlib)
file(GLOB srcs png*.c)
add_library(yack-png STATIC ${srcs})

add_executable(yack_pngtest tests/pngtest.c)
target_link_libraries(yack_pngtest yack-png yack-z)
