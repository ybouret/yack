
#include "yack/concurrent/loop/runnable.hpp"

namespace yack
{
    namespace concurrent
    {

        runnable:: ~runnable() throw()
        {

        }

        runnable:: runnable() throw()
        {
        }

        void runnable:: call(const context &here,
                             void          *args,
                             lockable      &sync) throw()
        {
            assert(args);
            static_cast<runnable*>(args)->run(here,sync);
        }
    }

}

