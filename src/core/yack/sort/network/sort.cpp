
#include "yack/sort/network/sort.hpp"

namespace yack {

    const char * const network_sort:: call_sign = "network_sort";

    network_sort:: ~network_sort() noexcept
    {}

#define COMMA ,
    network_sort:: network_sort() noexcept : YACK_NETWORK_SORT_REP(IMPL,COMMA)
    {
    }
    

}
