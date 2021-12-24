//! \file

#ifndef YACK_RANDOMIZED_PARK_MILLER_INCLUDED
#define YACK_RANDOMIZED_PARK_MILLER_INCLUDED 1

#include "yack/randomized/bits.hpp"

namespace yack
{
    
    namespace randomized
    {

        //! Park and Miller's
        class ParkMiller : public bits
        {
        public:
            virtual ~ParkMiller() throw();                  //!< cleanup
            explicit ParkMiller(const long seed=0) throw(); //!< setup
            virtual uint32_t next32() throw();
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ParkMiller);
            long word;
        };
        
    }
    
}

#endif
