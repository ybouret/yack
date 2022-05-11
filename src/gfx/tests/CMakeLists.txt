file(GLOB srcs test-*.cpp)
  
add_executable(test-gfx main.cpp ${srcs})

target_link_libraries(test-gfx yack-gfx yack-jive yack-png yack-z yack-jpeg yack-tiff)
yack_link_libraries(test-gfx)

yack_testing(gfx)


set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()

add_test(types      test-gfx types)
add_test(options    test-gfx options)
add_test(tess       test-gfx tess)
add_test(hist       test-gfx hist)
add_test(broker     test-gfx broker)
add_test(png_io     test-gfx png_io)
add_test(bmp_io     test-gfx bmp_io)
add_test(png        test-gfx png)
add_test(jpg        test-gfx jpg)
add_test(format     test-gfx format)





