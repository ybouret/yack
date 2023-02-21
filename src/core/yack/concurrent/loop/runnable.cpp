
#include "yack/concurrent/loop/runnable.hpp"

namespace yack
{
    namespace concurrent
    {

        runnable:: ~runnable() noexcept
        {

        }

        runnable:: runnable() noexcept
        {
        }

        void runnable:: call(const context &here,
                             void          *args,
                             lockable      &sync) noexcept
        {
            assert(args);
            static_cast<runnable*>(args)->run(here,sync);
        }
    }

}

