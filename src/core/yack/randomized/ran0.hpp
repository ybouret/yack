

//! \file

#ifndef YACK_RANDOMIZED_RAN0_INCLUDED
#define YACK_RANDOMIZED_RAN0_INCLUDED 1

#include "yack/randomized/ubits.hpp"

namespace yack
{
    
    namespace randomized
    {
        
        class ran0 : public bits_on<long>
        {
        public:
            virtual ~ran0() throw();
            explicit ran0(const long seed) throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ran0);
            virtual long next() throw();
            long word;
        };
        
    }
    
}

#endif
