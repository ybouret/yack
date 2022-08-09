
#include "yack/net/channel.hpp"

namespace yack
{
    namespace net
    {
        channel::  channel(const socket_address &srv) throw() : host(srv) {}
        channel:: ~channel() throw() {}
    }
}


