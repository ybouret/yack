
add_executable(zlib_example example.c)
target_link_libraries(zlib_example yack-z)

add_custom_command( TARGET zlib_example
POST_BUILD
COMMAND zlib_example
COMMENT "[YACK.ZLIB] example...")

add_executable(minigzip minigzip.c)
target_link_libraries(minigzip yack-z)
