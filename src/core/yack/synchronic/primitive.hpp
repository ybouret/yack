//! \file

#ifndef YACK_SYNC_PRIMITIVE_INCLUDED
#define YACK_SYNC_PRIMITIVE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace synchronic
    {

        //______________________________________________________________________
        //
        //
        //! a primtive synchronisation object
        //
        //______________________________________________________________________
        class primitive
        {
        public:
            static void display();           //!< display internal info
            virtual    ~primitive() throw(); //!< cleanup

        protected:
            explicit    primitive() throw(); //!< setup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(primitive);
        };
    }
}

#endif

