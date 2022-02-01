file(GLOB srcs test-*.cpp)
 
add_executable(test-chem main.cpp ${srcs})

target_link_libraries(test-chem yack-chem yack-jive yack-lua++ yack-math yack-lua)
yack_link_libraries(test-chem)

yack_testing(chem)

enable_testing()

add_test(species test-chem species)
add_test(library test-chem library)
add_test(eq      test-chem eq)
add_test(eqs     test-chem eqs)
add_test(plexus  test-chem plexus)








