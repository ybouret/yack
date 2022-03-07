file(GLOB srcs test-*.cpp)
  
add_executable(test-gfx main.cpp ${srcs})

target_link_libraries(test-gfx yack-gfx yack-jive)
yack_link_libraries(test-gfx)

yack_testing(gfx)


set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()

add_test(types test-gfx types)






