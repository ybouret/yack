include_directories(. ../core)

set(nw_decl yack/network.hpp)
set(nw_srcs yack/network.cpp)

install( FILES ${nw_decl} DESTINATION include/yack )

#main
file(GLOB net_main_decl yack/net/*.hpp)
file(GLOB net_main_srcs yack/net/*.cpp)
set(net_main ${net_main_decl} ${net_main_srcs})
SOURCE_GROUP(net FILES ${net_main})
install( FILES ${net_main_decl} DESTINATION include/yack/net )

#bsd
file(GLOB net_bsd_decl yack/net/bsd/*.hpp)
file(GLOB net_bsd_srcs yack/net/bsd/*.cpp)
set(net_bsd ${net_bsd_decl} ${net_bsd_srcs})
SOURCE_GROUP(net/bsd FILES ${net_bsd})
install( FILES ${net_bsd_decl} DESTINATION include/yack/net/bsd )

#socket
file(GLOB net_socket_decl yack/net/socket/*.hpp)
file(GLOB net_socket_srcs yack/net/socket/*.cpp)
set(net_socket ${net_socket_decl} ${net_socket_srcs})
SOURCE_GROUP(net/socket FILES ${net_socket})
install( FILES ${net_socket_decl} DESTINATION include/yack/net/socket )

add_library(yack-net STATIC
${nw_decl} ${nw_srcs}
${net_main}
${net_bsd}
${net_socket}
)

install( TARGETS yack-net ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(net)
