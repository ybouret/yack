
file(GLOB io io/*.cpp)
source_group(io FILES ${io} )
 

file(GLOB pattern pattern/*.cpp)
source_group(pattern FILES ${pattern} )

add_executable(test-jive main.cpp
${io}
${pattern}
)

target_link_libraries(test-jive yack-jive)
yack_link_libraries(test-jive)

yack_testing(jive)

enable_testing()

add_test(io:tags    test-jive tags)
add_test(io::module test-jive module)
add_test(io:token   test-jive token)

add_test(pattern:firsts test-jive firsts)
add_test(pattern        test-jive pattern)
add_test(pattern:viz    test-jive viz)
add_test(pattern:posix  test-jive posix)





