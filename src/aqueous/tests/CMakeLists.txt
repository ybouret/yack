file(GLOB srcs test-*.cpp)
 
add_executable(test-aqueous main.cpp ${srcs})

target_link_libraries(test-aqueous yack-aqueous yack-jive yack-lua++ yack-math yack-lua)
yack_link_libraries(test-aqueous)

yack_testing(aqueous)

add_test(lib    test-aqueous lib)
add_test(weasel test-aqueous weasel)
add_test(eqs    test-aqueous eqs)
add_test(lim    test-aqueous lim)
add_test(am     test-aqueous am)
add_test(realm  test-aqueous realm)



