file(GLOB src *.cpp)

add_executable(test-yack main.cpp ${src})
target_link_libraries(test-yack yack)
