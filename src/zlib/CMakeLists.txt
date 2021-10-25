
PROJECT(zlib C)
message( STATUS "[YACK.ZLIB]" )

message( STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}" )
string(REPLACE "-Wextra"
       "" CMAKE_C_FLAGS
       "${CMAKE_C_FLAGS}" )
       

file(GLOB zlib_srcs *.c)

set(zlib ${zlib_srcs})

add_library(yack-z ${zlib})
add_subdirectory(tests)

