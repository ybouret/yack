
set(YACK_KNOWN_COMPILER OFF)
set(YACK_COMPILERS      "")
get_filename_component(YACK_CC ${CMAKE_C_COMPILER} NAME_WE)

message( STATUS "found C compiler : '${YACK_CC}'" )

if(${YACK_CC} MATCHES "gcc.*" )
	set(YACK_KNOWN_COMPILER ON)
	set(YACK_COMPILERS      "gnu")
	
 	set(CMAKE_C_FLAGS   "-Wall -Wextra -pipe -fPIC")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pipe -fPIC -fexceptions -Weffc++" )
	
endif()

if(${YACK_CC} MATCHES "clang.*" )
	set(YACK_KNOWN_COMPILER ON)
	set(YACK_COMPILERS      "clang")
		
	set(CMAKE_C_FLAGS   "-Wall -Wextra -pipe -fPIC")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pipe -fPIC -fexceptions -Weffc++" )
	
endif()

if(${YACK_CC} MATCHES "icc.*" )
	set(YACK_KNOWN_COMPILER ON)
	set(YACK_COMPILERS      "intel")
		
	set(CMAKE_C_FLAGS   "-Wall -pipe -wd981 -fPIC -xHost")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -wd981 -fPIC -xHost -fexceptions" )
	
endif()

if(NOT YACK_KNOWN_COMPILER)
	message( FATAL_ERROR "unknown compilers '${YACK_CC}'" )
endif()

