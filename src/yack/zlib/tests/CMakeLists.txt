
include_directories(..)

add_executable(z-example example.c)
target_link_libraries(z-example yack-z)

add_custom_command( TARGET z-example
POST_BUILD
COMMAND z-example
COMMAND ${CMAKE_COMMAND} -E echo "[YACK]  <zlib example/>"
COMMENT "[YACK]  <zlib example>"
VERBATIM)
 
