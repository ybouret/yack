
//! \file

#ifndef YACK_CHEM_VAT_NORMALIZER_INCLUDED
#define YACK_CHEM_VAT_NORMALIZER_INCLUDED 1

#include "yack/chem/vat/guardian.hpp"
#include "yack/chem/vat/equalizer.hpp"

namespace yack
{
    namespace Chemical
    {

        class Normalizer
        {
        public:
            explicit Normalizer();
            virtual ~Normalizer() noexcept;

            //Custodian  custodian;
            //Equalizer  equalizer;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Normalizer);
        };

    }

}

#endif

