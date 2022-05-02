
//! \file

#ifndef YACK_GFX_BROKER_INCLUDED
#define YACK_GFX_BROKER_INCLUDED 1

#include "yack/gfx/tessellation.hpp"
#include "yack/concurrent/loop.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    namespace graphic
    {

        typedef arc_ptr<concurrent::loop> engine;

        typedef void (*gkernel)(void *, const tiles &, lockable &);

        class broker : public tessellation, public concurrent::runnable
        {
        public:
            explicit broker(const engine     &,
                            const dimensions &);

            virtual ~broker() throw();

            void operator()(gkernel, void *);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(broker);
            virtual void run(const concurrent::context &, lockable &) throw();
            engine  call;
            gkernel code;
            void   *args;

        };

    }

}

#endif
