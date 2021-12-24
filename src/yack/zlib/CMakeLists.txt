
PROJECT(zlib C)
message( STATUS "[YACK] <zlib>" )

yack_no_warnings()

file(GLOB zlib_srcs *.c)

set(zlib ${zlib_srcs})
set(zlib_public zlib.h zutil.h zconf.h)
install( FILES ${zlib_public} DESTINATION include/yack/zlib )


add_library(yack-z ${zlib})
install( TARGETS yack-z ARCHIVE DESTINATION lib )

add_subdirectory(tests)

message( STATUS "[YACK] <zlib/>" )
