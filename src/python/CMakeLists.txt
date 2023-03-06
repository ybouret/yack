
include_directories(../core)

add_library( engine MODULE engine.cpp )
yack_link_libraries(engine)

set(the_engine ${CMAKE_CURRENT_SOURCE_DIR}/engine.dll)
add_custom_command( TARGET engine 
POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:engine> ${the_engine}
DEPENDS engine
COMMENT "Copy local ${the_engine}"
)
