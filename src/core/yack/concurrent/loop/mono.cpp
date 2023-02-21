
#include "yack/concurrent/loop/mono.hpp"

namespace yack
{
    namespace concurrent
    {

        mono:: mono() noexcept : loop(), ctx(), acc() {}

        mono:: ~mono() noexcept
        {
            
        }

        size_t mono:: size() const noexcept { return 1; }

        loop::const_type & mono:: operator[](const size_t) const noexcept
        {
            return ctx;
        }


        lockable & mono:: access() noexcept { return acc; }

        const char mono:: clid[] = "mono";

        const char * mono:: family() const noexcept { return clid; }

        void mono:: operator()(kernel kcode, void *kargs) noexcept
        {
            assert(kcode);
            kcode(ctx,kargs,acc);
        }


    }

}
