file(GLOB srcs test-*.cpp)

#add_library(json-tests STATIC ${srcs})  
 

add_executable(test-json main.cpp ${srcs})

target_link_libraries(test-json yack-json yack-jive)
yack_link_libraries(test-json)

yack_testing(json)


set(HERE ${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()

add_test(db      test-json parser ${HERE}/db.js)
add_test(sample1 test-json parser ${HERE}/sample1.js)
add_test(sample2 test-json parser ${HERE}/sample2.js)
add_test(sample3 test-json parser ${HERE}/sample3.js)
add_test(sample4 test-json parser ${HERE}/sample4.js)






