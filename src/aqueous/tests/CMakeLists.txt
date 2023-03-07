file(GLOB srcs test-*.cpp)
 
add_executable(test-aqueous main.cpp ${srcs})

target_link_libraries(test-aqueous yack-chem yack-jive yack-lua++ yack-math yack-lua)
yack_link_libraries(test-aqueous)

yack_testing(aqueous)
