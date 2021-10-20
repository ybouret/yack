
//! \file

#ifndef YACK_RANDOMIZED_RAND_INCLUDED
#define YACK_RANDOMIZED_RAND_INCLUDED 1

#include "yack/randomized/ubits.hpp"

namespace yack
{

    namespace randomized
    {

        class system_rand : public bits_on<unsigned>
        {
        public:
            virtual ~system_rand() throw();
            explicit system_rand(const unsigned seed) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(system_rand);
            virtual unsigned next() throw();

        };

    }

}

#endif
