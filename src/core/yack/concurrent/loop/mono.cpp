
#include "yack/concurrent/loop/mono.hpp"

namespace yack
{
    namespace concurrent
    {

        mono:: mono() throw() : loop(), ctx(), acc() {}

        mono:: ~mono() throw()
        {
            
        }

        size_t mono:: size() const throw() { return 1; }

        loop::const_type & mono:: operator[](const size_t) const throw()
        {
            return ctx;
        }


        lockable & mono:: access() throw() { return acc; }

        const char mono:: clid[] = "mono";

        const char * mono:: family() const throw() { return clid; }

        void mono:: operator()(kernel kcode, void *kargs) throw()
        {
            assert(kcode);
            kcode(ctx,kargs,acc);
        }


    }

}
