file(GLOB srcs test-*.cpp)
 
add_executable(test-chem main.cpp ${srcs})

target_link_libraries(test-chem yack-chem yack-jive yack-lua++ yack-math yack-lua)
yack_link_libraries(test-chem)

yack_testing(chem)

enable_testing()

