
//! \file

#ifndef YACK_RANDOMIZED_BITS_ON_INCLUDED
#define YACK_RANDOMIZED_BITS_ON_INCLUDED 1

#include "yack/randomized/bits-on.hpp"

namespace yack
{

    namespace randomized
    {

        class rand_ : public bits_on<unsigned>
        {
        public:
            virtual ~rand_() throw();
            explicit rand_(const unsigned seed) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rand_);
            virtual unsigned next() throw();

        };

    }

}

#endif
