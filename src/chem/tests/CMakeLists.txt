file(GLOB srcs test-*.cpp)
 
add_executable(test-chem main.cpp ${srcs})

target_link_libraries(test-chem yack-jive yack-chem)
yack_link_libraries(test-chem)

yack_testing(chem)

enable_testing()







