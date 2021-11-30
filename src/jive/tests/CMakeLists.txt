
file(GLOB io io/*.cpp)
source_group(io FILES ${io} )
 


add_executable(test-jive main.cpp
${io}
 )

yack_link_libraries(test-jive)
target_link_libraries(test-jive yack-jive)

yack_testing(jive)

enable_testing()

add_test(io:token test-jive token)



