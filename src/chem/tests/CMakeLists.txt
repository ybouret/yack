file(GLOB srcs test-*.cpp)
 
add_executable(test-chem main.cpp ${srcs})

target_link_libraries(test-chem yack-chem yack-jive yack-lua++ yack-math yack-lua)
yack_link_libraries(test-chem)

yack_testing(chem)

add_test(lib        test-chem lib)
add_test(actors     test-chem actors)
add_test(components test-chem components)
add_test(outcome    test-chem outcome)
add_test(eq         test-chem eq)
add_test(composite  test-chem composite)
