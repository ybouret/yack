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

set(YACK_HAVE_CONFIG OFF)
if(YACK_DEBUG)
	set(YACK_CONFIG "Debug")
	set(YACK_HAVE_CONFIG ON)
endif()

if(YACK_RELEASE)
	set(YACK_CONFIG "Release")
	set(YACK_HAVE_CONFIG   ON)
endif()

if(FALSE)
if(NOT YACK_HAVE_CONFIG)
	message( FATAL_ERROR "[YACK] unknow config: use [Debug|Release]" )
endif()
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
	YACK_FIND_COMPILER_VERSION()
	
 	set(CMAKE_C_FLAGS   "-Wall -pipe -fPIC")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -fPIC -fexceptions -Weffc++" )
	
	if(${YACK_COMPILER_MAJOR} GREATER 4)
 		YACK_ADD_FLAGS("-Wextra")
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
	YACK_FIND_COMPILER_VERSION()

	set(CMAKE_C_FLAGS   "-Wall -Wextra -pipe -fPIC")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pipe -fPIC -fexceptions -Weffc++" )
	
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
	YACK_FIND_COMPILER_VERSION()

	set(CMAKE_C_FLAGS   "-Wall -pipe -wd981 -fPIC -xHost")
	set(CMAKE_CXX_FLAGS "-Wall -pipe -wd981 -fPIC -xHost -fexceptions" )
	
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
		
	set(CMAKE_C_FLAGS   "-nologo")
	set(CMAKE_CXX_FLAGS "-nologo -EHsc" )
	set(YACK_FLAGS_DEBUG   "")
	set(YACK_FLAGS_RELEASE "-Ox -DNDEBUG=1")
	 
endif()

if(NOT YACK_KNOWN_COMPILER)
	message( FATAL_ERROR "[YACK] unknown compilers '${YACK_CC}'" )
endif()

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
 	
	message( STATUS "[YACK] link libraries @${program} : ${yack_libs}" )
	target_link_libraries(${program} ${yack_libs})
endfunction()

