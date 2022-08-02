include_directories(. ../core)

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

#com
#file(GLOB net_com_decl yack/net/com/*.hpp)
#file(GLOB net_com_srcs yack/net/com/*.cpp)
#set(net_com ${net_com_decl} ${net_com_srcs})
#SOURCE_GROUP(net/com FILES ${net_com})
#install( FILES ${net_com_decl} DESTINATION include/yack/net/com )


#tcp
file(GLOB net_tcp_decl yack/net/tcp/*.hpp)
file(GLOB net_tcp_srcs yack/net/tcp/*.cpp)
set(net_tcp ${net_tcp_decl} ${net_tcp_srcs})
SOURCE_GROUP(net/tcp FILES ${net_tcp})
install( FILES ${net_tcp_decl} DESTINATION include/yack/net/tcp )

#udp
file(GLOB net_udp_decl yack/net/udp/*.hpp)
file(GLOB net_udp_srcs yack/net/udp/*.cpp)
set(net_udp ${net_udp_decl} ${net_udp_srcs})
SOURCE_GROUP(net/udp FILES ${net_udp})
install( FILES ${net_udp_decl} DESTINATION include/yack/net/udp )


add_library(yack-net STATIC
${net_main}
${net_bsd}
${net_socket}
#${net_com}
${net_tcp}
${net_udp}
)

install( TARGETS yack-net ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(net)
