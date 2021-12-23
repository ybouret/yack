project(png C)
include_directories(. ../zlib)
file(GLOB srcs png*.c)
add_library(yack-png STATIC ${srcs})

add_executable(yack_pngtest tests/pngtest.c)
target_link_libraries(yack_pngtest yack-png yack-z)

add_custom_command( TARGET yack_pngtest
POST_BUILD
COMMAND           $<TARGET_FILE:yack_pngtest>
DEPENDS           yack_pngtest
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
COMMENT           "[YACK]  <pngtest>")
