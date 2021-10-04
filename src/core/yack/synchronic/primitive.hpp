//! \file

#ifndef YACK_SYNC_PRIMITIVE_INCLUDED
#define YACK_SYNC_PRIMITIVE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace synchronic
    {
        class primitive
        {
        public:
            virtual ~primitive() throw();
            
            static void display();
            
        protected:
            explicit primitive() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(primitive);
        };
    }
}

#endif

