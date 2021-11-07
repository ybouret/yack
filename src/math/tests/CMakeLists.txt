
file(GLOB algebra algebra/*.cpp)
source_group(algebra FILES ${algebra} )

add_executable(test-math main.cpp
${algebra}
)
yack_link_libraries(test-math)

