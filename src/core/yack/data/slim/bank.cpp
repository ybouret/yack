#include "yack/data/slim/bank.hpp"

namespace yack
{
    namespace kernel
    {
        const char * const slim_bank:: clid = "slim_bank";

        slim_bank:: ~slim_bank() throw()
        {
        }

        slim_bank:: slim_bank() throw() :
        object(), counted(), releasable(), concurrent::mutex(clid)
        {

        }
    }
}
