
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/limiting.hpp"
#include "yack/chem/boundary.hpp"

namespace yack
{

    namespace chemical
    {
        class equalizer
        {
        public:
            equalizer(const size_t m, const sp_fund &io);
            ~equalizer() throw();

            void reset() throw();

            limiting   pos;
            boundaries neg;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equalizer);
        };

    }

}

#endif

