file(GLOB srcs test-*.cpp)

add_executable(test-csv main.cpp ${srcs})

target_link_libraries(test-csv yack-csv yack-jive)
yack_link_libraries(test-csv)

yack_testing(csv)


set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
#enable_testing()







