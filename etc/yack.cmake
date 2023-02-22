########################################################################
##
##
##  Global Setup
##
##
########################################################################
set(CMAKE_SKIP_PREPROCESSED_SOURCE_RULES ON)
set(CMAKE_SKIP_ASSEMBLY_SOURCE_RULES     ON)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS    ON)

########################################################################
##
##
##  Find Configuration
##
##
########################################################################

string(COMPARE EQUAL "Debug"    "${CMAKE_BUILD_TYPE}" YACK_DEBUG)
string(COMPARE EQUAL "Release"  "${CMAKE_BUILD_TYPE}" YACK_RELEASE)

if(YACK_DEBUG OR YACK_RELEASE)
	message( STATUS "[YACK] CMAKE_BUILD_TYPE is valid and defined") 
else()
	message( STATUS "[YACK] CMAKE_BUILD_TYPE is set to 'Debug'" )
	set(CMAKE_BUILD_TYPE "Debug")
	set(YACK_DEBUG ON)
endif()

	

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

message( STATUS "[YACK] cmake config     : '${CMAKE_BUILD_TYPE}'" )
message( STATUS "[YACK] system name      : '${CMAKE_SYSTEM_NAME}'" )
message( STATUS "[YACK] found C compiler : '${YACK_CC}'" )
message( STATUS "[YACK] system processor : '${CMAKE_HOST_SYSTEM_PROCESSOR}'" )
message( STATUS "[YACK] sizeof(void*)    : '${CMAKE_SIZEOF_VOID_P}'" )

string(COMPARE EQUAL "Darwin"  ${CMAKE_SYSTEM_NAME} YACK_DARWIN)
string(COMPARE EQUAL "FreeBSD" ${CMAKE_SYSTEM_NAME} YACK_FREEBSD)
string(COMPARE EQUAL "Linux"   ${CMAKE_SYSTEM_NAME} YACK_LINUX)
string(COMPARE EQUAL "SunOS"   ${CMAKE_SYSTEM_NAME} YACK_SUNOS)
string(COMPARE EQUAL "OpenBSD" ${CMAKE_SYSTEM_NAME} YACK_OPENBSD)
string(COMPARE EQUAL "Windows" ${CMAKE_SYSTEM_NAME} YACK_WINDOWS)


set(cmake_rm "")

if("${CMAKE_VERSION}" VERSION_LESS "3.17" )
	# old cmake -E
	message( STATUS "[YACK] removing with remove" )
	set(cmake_rm "remove")
else()
	message( STATUS "[YACK] removing with rm" )
	set(cmake_rm "rm")
endif()

#message( FATAL_ERROR "Testing" )




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
		message( FATAL_ERROR "[YACK] Couldn't find compiler version")
	endif()
	# get version pattern
	string( REGEX MATCH    "[0-9]+\\.[0-9]+\\.[0-9]+"         YC_VERSION "${YACK_RAW_COMPILER_VERSION}" )
	string( REGEX REPLACE  "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" YC_MAJOR   "${YC_VERSION}" )
	string( REGEX REPLACE  "[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" YC_MINOR   "${YC_VERSION}" )
	string( REGEX REPLACE  "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" YC_PATCH   "${YC_VERSION}" )
	message( STATUS "[YACK] '${YACK_CC}' is <${YC_VERSION}>: major=${YC_MAJOR}, minor=${YC_MINOR}, patchlevel=${YC_PATCH}" )
	set(YACK_COMPILER_VERSION ${YC_VERSION}  )
	set(YACK_COMPILER_MAJOR   ${YC_MAJOR}    )
	set(YACK_COMPILER_MINOR   ${YC_MINOR}    )
	set(YACK_COMPILER_PATCH   ${YC_PATCH}    )
endmacro(YACK_FIND_COMPILER_VERSION)

# macro to modify flags
macro(YACK_ADD_FLAGS flag)
	message( STATUS "[YACK] adding C/C++ FLAGS : '${flag}'" )
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
	SET(YACK_GNU            ON)
	YACK_FIND_COMPILER_VERSION()
	
 	set(CMAKE_C_FLAGS   "-Wall -pipe -fPIC -pedantic")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -fPIC -fexceptions -Weffc++" )
	
	if(${YACK_COMPILER_MAJOR} GREATER 4)
 		YACK_ADD_FLAGS("-Wextra")
	endif()
	
	if(${YACK_COMPILER_VERSION} VERSION_GREATER_EQUAL "4.8.1")
		YACK_ADD_FLAGS("-std=c++11")
	endif()
	
	set(YACK_FLAGS_DEBUG   "-O0 -g")
	set(YACK_FLAGS_RELEASE "-O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
	 
	
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
	set(YACK_CLANG          ON)
	YACK_FIND_COMPILER_VERSION()

	set(CMAKE_C_FLAGS   "-Wall -Wextra -pipe -fPIC")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pipe -fPIC -fexceptions -Weffc++ -std=c++11" )
	
	set(YACK_FLAGS_DEBUG   "-O0 -g")
	set(YACK_FLAGS_RELEASE "-O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
	 
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
	set(YACK_INTEL          ON)
	YACK_FIND_COMPILER_VERSION()

	set(CMAKE_C_FLAGS   "-Wall -pipe -wd981 -fPIC -xHost")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -wd981 -fPIC -xHost -fexceptions -std=c++11" )
	
	set(YACK_FLAGS_DEBUG   "-O0 -g")
	set(YACK_FLAGS_RELEASE "-O2 -DNDEBUG=1 -D_FORTIFY_SOURCE=2")
	
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
	set(YACK_MICROSOFT      ON)
	
	set(CMAKE_C_FLAGS   "-nologo")
	set(CMAKE_CXX_FLAGS "-nologo -EHsc" )
	set(YACK_FLAGS_DEBUG   "")
	set(YACK_FLAGS_RELEASE "-Ox -DNDEBUG=1")
	 
endif()

if(NOT YACK_KNOWN_COMPILER)
	message( FATAL_ERROR "[YACK] unknown compilers '${YACK_CC}'" )
endif()

########################################################################
##
##
##  updating flags
##
##
########################################################################
set(CMAKE_C_FLAGS_RELEASE   ${YACK_FLAGS_RELEASE})
set(CMAKE_CXX_FLAGS_RELEASE ${YACK_FLAGS_RELEASE})
set(CMAKE_C_FLAGS_DEBUG     ${YACK_FLAGS_DEBUG})
set(CMAKE_CXX_FLAGS_DEBUG   ${YACK_FLAGS_DEBUG})


########################################################################
##
##
##  linking
##
##
########################################################################

function(yack_link_libraries program)
	set(yack_libs "yack")
	
	########################################################################
	##
	## threads support
	##
	########################################################################
	if(YACK_LINUX OR YACK_FREEBSD)
		list(APPEND yack_libs "pthread")
	endif()
	
	if(YACK_LINUX)
		list(APPEND yack_libs "rt")
		list(APPEND yack_libs "dl")
	endif()

	if(YACK_WINDOWS)
		if(YACK_GNU)
			target_link_libraries(${program} -static-libgcc -static-libstdc++) 
		endif()
	endif()

 	
	message( STATUS "[YACK] link libraries @${program} : ${yack_libs}" )
	target_link_libraries(${program} ${yack_libs})
endfunction()

# check if we need to explicitly link libm for a C program
set(YACK_NEEDS_LIBM OFF)
if(YACK_WINDOWS)
	if(NOT YACK_MICROSOFT)
		set(YACK_NEEDS_LIBM ON)
	endif()
else()
		set(YACK_NEEDS_LIBM ON)
endif()

macro(yack_no_warnings)
if(YACK_GNU OR YACK_CLANG)
	string(REPLACE "-Wall"   "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	string(REPLACE "-Wextra" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	string(STRIP "${CMAKE_C_FLAGS}" CMAKE_C_FLAGS)
endif()
endmacro(yack_no_warnings)


