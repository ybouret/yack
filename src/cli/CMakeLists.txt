add_executable(yack_leq yack_leq.cpp)

include_directories(../core)
add_executable(yack_uniq yack_uniq.cpp)
target_link_libraries(yack_uniq yack)

install( TARGETS yack_leq  RUNTIME DESTINATION bin )
install( TARGETS yack_uniq RUNTIME DESTINATION bin )
