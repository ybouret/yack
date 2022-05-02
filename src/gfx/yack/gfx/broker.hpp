
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
        //______________________________________________________________________
        //
        //
        //! alias
        //
        //______________________________________________________________________
        typedef arc_ptr<concurrent::loop> engine;

        //______________________________________________________________________
        //
        //
        //! graphic kernel prototype
        //
        //______________________________________________________________________
        typedef void (*commands)(void *, const tiles &, lockable &);

        //______________________________________________________________________
        //
        //
        //! graphic kernels broker
        //
        //______________________________________________________________________
        class broker : public tessellation, public concurrent::runnable
        {
        public:
            //! setup with an engine and dimensions
            explicit broker(const engine     &,
                            const dimensions &);

            //! cleanup
            virtual ~broker() throw();

            //! call gkernel on each set of tiles
            void operator()(commands, void *);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(broker);
            virtual void run(const concurrent::context &, lockable &) throw();
            engine   call;
            commands code;
            void    *args;

        };

    }

}

#endif
