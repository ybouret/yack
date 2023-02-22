
include_directories(../core)

add_library( dsoxx MODULE dsoxx.cpp )

set(the_dll ${CMAKE_CURRENT_SOURCE_DIR}/dsoxx.dll)

add_custom_command( TARGET dsoxx 
COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dsoxx> ${the_dll}
DEPENDS dsoxx
COMMENT "Copy local ${the_dll}"
)

add_library( engine MODULE engine.cpp )
yack_link_libraries(engine)

