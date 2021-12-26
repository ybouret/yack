project(png C)

yack_no_warnings()

include_directories(. ../zlib)
file(GLOB srcs png*.c)
add_library(yack-png STATIC ${srcs})

add_executable(yack_pngtest tests/pngtest.c)
target_link_libraries(yack_pngtest yack-png yack-z)


if(YACK_NEEDS_LIBM)
	target_link_libraries(yack_pngtest m)
endif()

add_custom_command( TARGET yack_pngtest
POST_BUILD
COMMAND           $<TARGET_FILE:yack_pngtest>
COMMAND           ${CMAKE_COMMAND} -E ${cmake_rm} -f pngout.png
DEPENDS           yack_pngtest
COMMAND           ${CMAKE_COMMAND} -E echo "[YACK] done [png]"
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
COMMENT           "[YACK] test [png]"
VERBATIM)
