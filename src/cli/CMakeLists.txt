add_executable(yack_leq yack_leq.cpp)

include_directories(../core)
add_executable(yack_uniq yack_uniq.cpp)
yack_link_libraries(yack_uniq)

#add_executable(yack_irish yack_irish.cpp)
#yack_link_libraries(yack_irish)

#add_executable(yack_wallpapers yack_wallpapers.cpp)
#yack_link_libraries(yack_wallpapers)

add_executable(yack_nwsort yack_nwsort.cpp)
yack_link_libraries(yack_nwsort)

install( TARGETS yack_leq  RUNTIME DESTINATION bin )
install( TARGETS yack_uniq RUNTIME DESTINATION bin )
#install( TARGETS yack_irish RUNTIME DESTINATION bin )
#install( TARGETS yack_wallpapers RUNTIME DESTINATION bin)
