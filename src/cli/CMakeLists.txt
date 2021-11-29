add_executable(yack_leq yack_leq.cpp)

include_directories(../core)
add_executable(yack_uniq yack_uniq.cpp)
yack_link_libraries(yack_uniq)

install( TARGETS yack_leq  RUNTIME DESTINATION bin )
install( TARGETS yack_uniq RUNTIME DESTINATION bin )
