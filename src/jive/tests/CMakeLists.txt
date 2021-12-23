
file(GLOB io io/*.cpp)
source_group(io FILES ${io} )
 

file(GLOB pattern pattern/*.cpp)
source_group(pattern FILES ${pattern} )

file(GLOB lexical lexical/*.cpp)
source_group(lexical FILES ${lexical} )

file(GLOB syntax syntax/*.cpp)
source_group(syntax FILES ${syntax} )

add_executable(test-jive main.cpp
${io}
${pattern}
${lexical}
${syntax}
)

target_link_libraries(test-jive yack-jive)
yack_link_libraries(test-jive)

yack_testing(jive)

enable_testing()

add_test(io:tags    test-jive tags)
add_test(io::module test-jive module)
add_test(io:token   test-jive token)

add_test(pattern:firsts   test-jive firsts)
add_test(pattern          test-jive pattern)
add_test(pattern:viz      test-jive viz)
add_test(pattern:posix    test-jive posix)
add_test(pattern:regexp   test-jive regexp)
add_test(pattern:matching test-jive matching)

add_test(lexical:scanner  test-jive scanner)
add_test(lexical:analyzer test-jive analyzer)

add_test(syntax:types     test-jive syntax_types)







