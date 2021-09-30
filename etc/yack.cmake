########################################################################
##
##
##  Find Compiler Family
##
##
########################################################################
set(YACK_KNOWN_COMPILER OFF)
set(YACK_COMPILERS      "")
get_filename_component(YACK_CC ${CMAKE_C_COMPILER} NAME_WE)

message( STATUS "found C compiler : '${YACK_CC}'" )

########################################################################
##
##
##  Find Compiler Version, variables and macro
##
##
########################################################################
set(YACK_COMPILER_VERSION "")
set(YACK_COMPILER_MAJOR   "")
set(YACK_COMPILER_MINOR   "")
set(YACK_COMPILER_PATCH   "")

# macro will be call for gnu/clang/intel...
macro(YACK_FIND_COMPILER_VERSION)
	# call cc --version
	execute_process(COMMAND ${CMAKE_C_COMPILER} --version
					OUTPUT_VARIABLE YACK_RAW_COMPILER_VERSION
					RESULT_VARIABLE YACK_FIND_COMPILER_VERSION_SUCCESS
					ERROR_QUIET
					)
	if( NOT ("0" STREQUAL "${YACK_FIND_COMPILER_VERSION_SUCCESS}") )
		message( FATAL_ERROR "Couldn't find compiler version")
	endif()
	# get version pattern
	string( REGEX MATCH    "[0-9]+\\.[0-9]+\\.[0-9]+"         YC_VERSION "${YACK_RAW_COMPILER_VERSION}" )
	string( REGEX REPLACE  "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" YC_MAJOR   "${YC_VERSION}" )
	string( REGEX REPLACE  "[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" YC_MINOR   "${YC_VERSION}" )
	string( REGEX REPLACE  "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" YC_PATCH   "${YC_VERSION}" )
	message( STATUS "[${YACK_CC}] is <${YC_VERSION}>: major=${YC_MAJOR}, minor=${YC_MINOR}, patchlevel=${YC_PATCH}" )
	set(YACK_COMPILER_VERSION ${YC_VERSION}  )
	set(YACK_COMPILER_MAJOR   ${YC_MAJOR}    )
	set(YACK_COMPILER_MINOR   ${YC_MINOR}    )
	set(YACK_COMPILER_PATCH   ${YC_PATCH}    )
endmacro(YACK_FIND_COMPILER_VERSION)

# macro to modify flags
macro(YACK_ADD_FLAGS flag)
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
endmacro()

########################################################################
##
##
##  GNU
##
##
########################################################################

if(${YACK_CC} MATCHES "gcc.*" )

	set(YACK_KNOWN_COMPILER ON)
	set(YACK_COMPILERS      "gnu")
	YACK_FIND_COMPILER_VERSION()
	
 	set(CMAKE_C_FLAGS   "-Wall -pipe -fPIC")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -fPIC -fexceptions -Weffc++" )
	
	if(${YACK_COMPILER_VERSION} GREATER 4)
		YACK_ADD_FLAGS("-Wextra")
	endif()
endif()

########################################################################
##
##
##  Clang
##
##
########################################################################
if(${YACK_CC} MATCHES "clang.*" )

	set(YACK_KNOWN_COMPILER ON)
	set(YACK_COMPILERS      "clang")
	YACK_FIND_COMPILER_VERSION()

	set(CMAKE_C_FLAGS   "-Wall -Wextra -pipe -fPIC")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pipe -fPIC -fexceptions -Weffc++" )
	
endif()

########################################################################
##
##
##  Intel
##
##
########################################################################
if(${YACK_CC} MATCHES "icc.*" )

	set(YACK_KNOWN_COMPILER ON)
	set(YACK_COMPILERS      "intel")
	YACK_FIND_COMPILER_VERSION()

	set(CMAKE_C_FLAGS   "-Wall -pipe -wd981 -fPIC -xHost")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -wd981 -fPIC -xHost -fexceptions" )
	
endif()

########################################################################
##
##
##  Microsoft
##
##
########################################################################
if(${YACK_CC} STREQUAL "cl" )

	set(YACK_KNOWN_COMPILER ON)
	set(YACK_COMPILERS      "microsoft")
		
	set(CMAKE_C_FLAGS   "-nologo")
	set(CMAKE_CXX_FLAGS "-nologo -EHsc" )
	
endif()

if(NOT YACK_KNOWN_COMPILER)
	message( FATAL_ERROR "unknown compilers '${YACK_CC}'" )
endif()

