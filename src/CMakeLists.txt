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
	COMMAND ${CMAKE_CTEST_COMMAND} #-C ${YACK_CONFIG}
	DEPENDS test-${target}
	COMMENT "[YACK] Running Unit Tests for <${target}>" )	
endmacro()

add_subdirectory(core)

string( STRIP "${UNIT_TESTS}" UNIT_TESTS) 
message( STATUS "[YACK] Unit Tests: '${UNIT_TESTS}'" )

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

