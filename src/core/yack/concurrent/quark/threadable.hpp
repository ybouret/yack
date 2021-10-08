

//! \file

#ifndef YACK_SYNC_QUARK_THREADABLE_INCLUDED
#define YACK_SYNC_QUARK_THREADABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace concurrent
    {

        namespace quark
        {
            //__________________________________________________________________
            //
            //
            //! threadable interface
            //
            //__________________________________________________________________
            class threadable
            {
            public:

                typedef void (*procedure)(void *);                    //!< legacy C wrapper
                explicit       threadable(procedure, void *) throw(); //!< setup
                virtual       ~threadable() throw();                  //!< cleanup
                void           run(void);                             //!< call

            protected:
                procedure   proc; //!< code address
                void       *args; //!< arguments

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(threadable);

            };
        }
    }

}

#endif
