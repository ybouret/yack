
PROJECT(zlib C)
message( STATUS "[YACK] <zlib>" )

if(YACK_GNU)
	message( STATUS "[YACK]   cleaning compile flags" )
	string(REPLACE "-Wextra"
			"" CMAKE_C_FLAGS
			"${CMAKE_C_FLAGS}" )
endif()

file(GLOB zlib_srcs *.c)

set(zlib ${zlib_srcs})
set(zlib_public zlib.h zutil.h zconf.h)
install( FILES ${zlib_public} DESTINATION include/yack/zlib )


add_library(yack-z ${zlib})
install( TARGETS yack-z ARCHIVE DESTINATION lib )

add_subdirectory(tests)

message( STATUS "[YACK] <zlib/>" )
