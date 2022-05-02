#include "yack/gfx/broker.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace  graphic
    {

        broker:: ~broker() throw() {}

        broker:: broker(const engine     &user,
                        const dimensions &dims) :
        tessellation(dims,user->size()),
        call(user),
        code(NULL),
        args(NULL)
        {
        }

        void broker:: operator()(gkernel user_code, void *user_args)
        {
            const temporary<gkernel> keep_code(code,user_code);
            const temporary<void *>  keep_args(args,user_args);
            call->run(*this);
        }

        void broker:: run(const concurrent::context &ctx, lockable &sync) throw()
        {
            assert(code);
            const tessellation &tess = *this;
            code(args,tess[ctx.indx],sync);
        }

    }
}
