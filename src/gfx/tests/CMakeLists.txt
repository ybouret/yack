file(GLOB srcs test-*.cpp)
  
add_executable(test-gfx main.cpp ${srcs})

target_link_libraries(test-gfx yack-gfx yack-png yack-z)
yack_link_libraries(test-gfx)

yack_testing(gfx)


set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()

add_test(types      test-gfx types)
add_test(rgba_field test-gfx rgba_field)
add_test(png_io     test-gfx png_io)
add_test(png        test-gfx png)






