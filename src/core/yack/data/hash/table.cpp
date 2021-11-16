#include "yack/data/hash/table.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    void  hash_table_:: check(const size_t user_exp2, const size_t maxi_exp2)
    {
        if(user_exp2>maxi_exp2) throw libc::exception(ENOMEM,"too many required slots");
    }

}
