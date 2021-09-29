project(yack)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../sdk" )

#list of registered unit tests
set(UNIT_TESTS "")

# append utest-target to list
macro(yack_utest target)
	set(UNIT_TESTS "${UNIT_TESTS} utest-${target}" PARENT_SCOPE)
endmacro()

macro(yack_testing target)
	add_custom_target(utest-${target}
	COMMAND ${CMAKE_CTEST_COMMAND}
	DEPENDS test-${target}
	COMMENT "Testing ${target}" )	
endmacro()

add_subdirectory(core)

string( STRIP "${UNIT_TESTS}" UNIT_TESTS) 
message( STATUS "Unit Tests: '${UNIT_TESTS}'" )
