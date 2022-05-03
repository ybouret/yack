
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
        // aliases
        //
        //______________________________________________________________________
        typedef arc_ptr<concurrent::loop> engine;  //!< shared concurrent loop
        typedef concurrent::context       context; //!< concurrent context


        //______________________________________________________________________
        //
        //
        //! graphic kernels broker
        //
        //______________________________________________________________________
        class broker : public tessellation, public concurrent::runnable
        {
        public:

            //! definition of commands to perform
            typedef void (*commands)(void *,const tiles &, const context &, lockable &);

            //! setup with an engine and dimensions
            explicit broker(const engine     &,
                            const dimensions &);

            //! cleanup
            virtual ~broker() throw();

            //! call gkernel on each set of tiles
            void operator()(commands, void *);

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(broker);
            virtual void run(const context &, lockable &) throw();
            engine   call;
            commands code;
            void    *args;

        };

    }

}

#endif
