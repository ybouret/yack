project(jpeg C)

string(REPLACE "-Wall"   "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REPLACE "-Wextra" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(STRIP "${CMAKE_C_FLAGS}" CMAKE_C_FLAGS)

file(GLOB srcs j*.c)

include_directories(.)

add_library(yack-jpeg STATIC ${srcs})

# cjpeg
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
COMMAND           ${CMAKE_COMMAND} -E echo "[YACK] done [cjpeg]"
DEPENDS           yack_cjpeg
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
COMMENT           "[YACK] test [cjpeg]"
VERBATIM)


#djepeg
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

add_custom_command( TARGET yack_djpeg
POST_BUILD
COMMAND           ${CMAKE_COMMAND} -E ${cmake_rm} -f testout.ppm testout.gif testout.bmp testoutp.ppm
COMMAND           $<TARGET_FILE:yack_djpeg> -dct int -ppm -outfile testout.ppm testorig.jpg
COMMAND           ${CMAKE_COMMAND} -E compare_files testout.ppm  testimg.ppm
COMMAND           $<TARGET_FILE:yack_djpeg> -dct int -gif -outfile testout.gif testorig.jpg
COMMAND           ${CMAKE_COMMAND} -E compare_files testout.gif  testimg.gif
COMMAND           $<TARGET_FILE:yack_djpeg> -dct int -bmp -colors 256 -outfile testout.bmp testorig.jpg
COMMAND           ${CMAKE_COMMAND} -E compare_files testout.bmp  testimg.bmp
COMMAND           $<TARGET_FILE:yack_djpeg> -dct int -ppm -outfile testoutp.ppm testprog.jpg
COMMAND           ${CMAKE_COMMAND} -E compare_files testoutp.ppm  testimg.ppm
COMMAND           ${CMAKE_COMMAND} -E ${cmake_rm} -f testout.ppm testout.gif testout.bmp testoutp.ppm 
COMMAND           ${CMAKE_COMMAND} -E echo "[YACK] done [djpeg]"
DEPENDS           yack_djpeg
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
COMMENT           "[YACK] test [djpeg]"
VERBATIM)


# jpegtran
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
COMMAND           ${CMAKE_COMMAND} -E echo "[YACK] done [jpegtran]"
DEPENDS           yack_jpegtran
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
COMMENT           "[YACK] test [jpegtran]"
VERBATIM)

#example for I/O
#add_executable(yack_jpeg_example tests/example.c)
#target_link_libraries(yack_jpeg_example yack-jpeg)

