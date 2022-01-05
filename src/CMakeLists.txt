project(yack)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../sdk" )

include(../etc/yack.cmake)

#list of registered unit tests
set(UNIT_TESTS "")

# append utest-target to list
macro(yack_utest target)
	set(UNIT_TESTS "${UNIT_TESTS} utest-${target}" PARENT_SCOPE)
endmacro()

#define a local target for unit tests
macro(yack_testing target)
	add_custom_target(utest-${target}
	COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG>
	DEPENDS test-${target}
	COMMENT "[YACK] Running Unit Tests for <${target}>" )	
endmacro()

add_subdirectory(core)

# check documentation generation
find_program(doxygen doxygen)
if("${doxygen}" STREQUAL "doxygen-NOTFOUND")
	message( STATUS "[YACK] no possible documentation" )
else()
	message( STATUS "[YACK] creating documentation target" )
add_custom_target(dox
	COMMAND ${doxygen}
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.."
	COMMENT "[YACK] Creating Documentation with Doxygen")
endif()


SET(THIRD_PARTY "")
macro(yack_third_party target)
	set(THIRD_PARTY "${THIRD_PARTY} ${target}" PARENT_SCOPE)
endmacro()

add_subdirectory(cli)
add_subdirectory(yack/zlib)
add_subdirectory(yack/bzip2)
add_subdirectory(yack/lua)
add_subdirectory(math)
add_subdirectory(lua++)
add_subdirectory(jive)
add_subdirectory(json)
add_subdirectory(yack/png)
add_subdirectory(yack/jpeg)
add_subdirectory(yack/jbig)
add_subdirectory(yack/sqlite3)

# creating regression from unit tests
string( STRIP "${UNIT_TESTS}" UNIT_TESTS)
string( REPLACE " " ";" UNIT_TESTS "${UNIT_TESTS}" )



message( STATUS "[YACK] Unit Tests  :" )
foreach(t IN LISTS UNIT_TESTS)
	message( STATUS "     |_${t}")
endforeach()

add_custom_target(regression 
	DEPENDS ${UNIT_TESTS}
	COMMENT "[YACK] Performed Regression")
	
# creating third party
string( STRIP "${THIRD_PARTY}" THIRD_PARTY)
string( REPLACE " " ";" THIRD_PARTY "${THIRD_PARTY}" )

message( STATUS "[YACK] Third Party :" )
foreach(t IN LISTS THIRD_PARTY)
	message( STATUS "     |_${t}")
endforeach()

add_custom_target(third_party 
	DEPENDS ${THIRD_PARTY}
	COMMENT "[YACK] Built Third Party")
