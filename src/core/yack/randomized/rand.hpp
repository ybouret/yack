
//! \file

#ifndef YACK_RANDOMIZED_BITS_ON_INCLUDED
#define YACK_RANDOMIZED_BITS_ON_INCLUDED 1

#include "yack/randomized/bits.hpp"

namespace yack
{

    namespace randomized
    {
        //______________________________________________________________________
        //
        //
        //! bits based on system RAND
        //
        //______________________________________________________________________
        class rand_ : public bits
        {
        public:
            virtual ~rand_() throw(); //!< cleanup
            explicit rand_() throw(); //!< setup
            virtual uint32_t next32() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rand_);

        };

    }

}

#endif
