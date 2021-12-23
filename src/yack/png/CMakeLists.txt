project(png C)
include_directories(../zlib)
file(GLOB srcs png*.c)
add_library(png STATIC ${srcs})


