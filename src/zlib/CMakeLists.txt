
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

add_library(yack-z ${zlib})
add_subdirectory(tests)

message( STATUS "[YACK] <zlib/>" )
