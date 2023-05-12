
file(GLOB io io/*.cpp)
source_group(io FILES ${io} )
 

file(GLOB pattern pattern/*.cpp)
source_group(pattern FILES ${pattern} )

file(GLOB lexical lexical/*.cpp)
source_group(lexical FILES ${lexical} )

file(GLOB syntax syntax/*.cpp)
source_group(syntax FILES ${syntax} )

file(GLOB dsl dsl/*.cpp)
source_group(dsl FILES ${dsl} )

add_executable(test-jive main.cpp
${io}
${pattern}
${lexical}
${syntax}
${dsl}
)

target_link_libraries(test-jive yack-jive)
yack_link_libraries(test-jive)

yack_testing(jive)

enable_testing()

add_test(tags     test-jive tags)
add_test(module   test-jive module)
add_test(token    test-jive token)

add_test(firsts   test-jive firsts)
add_test(pattern  test-jive pattern)
add_test(viz      test-jive viz)
add_test(posix    test-jive posix)
add_test(regexp   test-jive regexp)
add_test(matching test-jive matching)
add_test(spec     test-jive spec)

add_test(scanner  test-jive scanner)
add_test(analyzer test-jive analyzer)

add_test(syntax:types test-jive syntax_types)
add_test(grammar      test-jive grammar)
add_test(parser       test-jive parser)
add_test(eval         test-jive eval)

add_test(vfs          test-jive vfs)
add_test(vfs:rm       test-jive vfs_rm)
add_test(dsl:parser   test-jive dsl_parser)
add_test(dsl:eval     test-jive dsl_eval)





