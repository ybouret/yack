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

add_custom_command( TARGET yack_cjpeg
POST_BUILD
COMMAND           ${CMAKE_COMMAND} -E ${cmake_rm} -f testout.jpg testoutp.jpg
COMMAND           $<TARGET_FILE:yack_cjpeg> -dct int -outfile                   testout.jpg  testimg.ppm
COMMAND           $<TARGET_FILE:yack_cjpeg> -dct int -progressive -opt -outfile testoutp.jpg testimg.ppm
COMMAND           ${CMAKE_COMMAND} -E compare_files testout.jpg  testimg.jpg
COMMAND           ${CMAKE_COMMAND} -E compare_files testoutp.jpg testimgp.jpg
COMMAND           ${CMAKE_COMMAND} -E ${cmake_rm} -f testout.jpg testoutp.jpg
DEPENDS           yack_cjpeg
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
COMMENT           "[YACK]  <cjpeg>"
VERBATIM)



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

add_custom_command( TARGET yack_jpegtran
POST_BUILD
COMMAND           ${CMAKE_COMMAND} -E ${cmake_rm} -f testoutt.jpg
COMMAND           $<TARGET_FILE:yack_jpegtran> -outfile testoutt.jpg testprog.jpg
COMMAND           ${CMAKE_COMMAND} -E compare_files testoutt.jpg testorig.jpg
COMMAND           ${CMAKE_COMMAND} -E ${cmake_rm} -f testoutt.jpg
DEPENDS           yack_jpegtran
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
COMMENT           "[YACK]  <jpegtran>"
VERBATIM)

