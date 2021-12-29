
include_directories(..)

add_executable(z-example example.c)
target_link_libraries(z-example yack-z)

add_custom_command( TARGET z-example
POST_BUILD
COMMAND z-example
COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --blue --bold "[YACK] done [zlib]"
COMMENT "[YACK] test [zlib]"
VERBATIM)
 
