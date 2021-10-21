

//! \file

#ifndef YACK_RANDOMIZED_RAN0_INCLUDED
#define YACK_RANDOMIZED_RAN0_INCLUDED 1

#include "yack/randomized/ubits.hpp"

namespace yack
{
    
    namespace randomized
    {

        //! Park and Miller's
        class ParkMiller : public bits_on<long>
        {
        public:
            virtual ~ParkMiller() throw();
            explicit ParkMiller(const long seed) throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ParkMiller);
            virtual long next() throw();
            long word;
        };
        
    }
    
}

#endif
