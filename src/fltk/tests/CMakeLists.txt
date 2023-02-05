
file(GLOB test_srcs test-*.cpp)
FLUID_UIC(matte)
FLUID_UIC(canvas)

add_executable(test-fltk main.cpp ${test_srcs} ${matte_SOURCES} ${canvas_SOURCES})
target_link_libraries(test-fltk yack-fltk)
yack_link_libraries(test-fltk)
#TARGET_LINK_YOCTO( test-fltk fltk)
TARGET_LINK_FLTK( test-fltk images)

