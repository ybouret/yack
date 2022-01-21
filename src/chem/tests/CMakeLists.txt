file(GLOB srcs test-*.cpp)
 
add_executable(test-chem main.cpp ${srcs})

target_link_libraries(test-chem yack-jive yack-chem)
yack_link_libraries(test-chem)

yack_testing(chem)

enable_testing()

add_test(species test-chem species)
add_test(library test-chem library)







