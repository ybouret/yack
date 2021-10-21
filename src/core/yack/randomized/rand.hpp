
//! \file

#ifndef YACK_RANDOMIZED_RAND_INCLUDED
#define YACK_RANDOMIZED_RAND_INCLUDED 1

#include "yack/randomized/ubits.hpp"

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
