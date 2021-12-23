project(jpeg C)

string(REPLACE "-Wall"   "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REPLACE "-Wextra" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(STRIP "${CMAKE_C_FLAGS}" CMAKE_C_FLAGS)

file(GLOB srcs j*.c)

include_directories(.)

add_library(yack-jpeg STATIC ${srcs})

add_executable(yack_cjpeg tests/cjpeg.c 
tests/rdbmp.c
tests/rdgif.c
tests/rdppm.c
tests/rdtarga.c
tests/rdswitch.c
tests/cdjpeg.c
)

target_link_libraries(yack_cjpeg yack-jpeg)


add_executable(yack_djpeg tests/djpeg.c
tests/cdjpeg.c
tests/wrbmp.c
tests/wrgif.c
tests/wrppm.c
tests/wrtarga.c
tests/rdswitch.c
tests/rdcolmap.c
)

target_link_libraries(yack_djpeg yack-jpeg)

add_executable(yack_jpegtran tests/jpegtran.c
tests/cdjpeg.c
tests/transupp.c
tests/rdswitch.c
)


target_link_libraries(yack_jpegtran yack-jpeg)
