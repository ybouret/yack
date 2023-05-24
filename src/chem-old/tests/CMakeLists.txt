file(GLOB srcs test-*.cpp)
 
add_executable(test-chem main.cpp ${srcs})

target_link_libraries(test-chem yack-chem yack-jive yack-lua++ yack-math yack-lua)
yack_link_libraries(test-chem)

yack_testing(chem)

add_test(lib    test-chem lib)
add_test(weasel test-chem weasel)
add_test(eqs    test-chem eqs)
add_test(lim    test-chem lim)
add_test(am     test-chem am)
add_test(coll   test-chem coll)
add_test(norm   test-chem norm)
add_test(realm  test-chem realm)
add_test(reactor test-chem reactor)
add_test(1d      test-chem 1d)
