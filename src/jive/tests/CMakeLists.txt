
file(GLOB io io/*.cpp)
source_group(io FILES ${io} )
 


add_executable(test-jive main.cpp
${io}
 )

target_link_libraries(test-jive yack-jive)
yack_link_libraries(test-jive)

yack_testing(jive)

enable_testing()

add_test(io:tags    test-jive tags)
add_test(io::source test-jive source)
add_test(io:token   test-jive token)



