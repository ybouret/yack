//! \file

#ifndef YACK_SYNC_CONTEXT_INCLUDED
#define YACK_SYNC_CONTEXT_INCLUDED 1

#include "yack/concurrent/split.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! context to locate a thread
        //
        //______________________________________________________________________
        class context
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            context()  throw();               //!< sequential
            ~context() throw();               //!< cleanup
            context(const size_t sz,
                    const size_t rk) throw(); //!< parallel

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t size; //!< size>0
            const size_t rank; //!< rank<size, MPI style
            const size_t indx; //!< indx=rank+1, C++ style

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(context);
        };
        
    }

}

#endif
