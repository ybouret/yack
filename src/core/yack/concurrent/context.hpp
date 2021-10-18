//! \file

#ifndef YACK_SYNC_CONTEXT_INCLUDED
#define YACK_SYNC_CONTEXT_INCLUDED 1

#include "yack/concurrent/split.hpp"
#include <iosfwd>

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

            friend std::ostream & operator<<(std::ostream &,const context&);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t size; //!< size>0
            const size_t rank; //!< rank<size, MPI style
            const size_t indx; //!< indx=rank+1, C++ style
            const size_t io10; //!< number of digits

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(context);
        };
        
    }

}

#endif
