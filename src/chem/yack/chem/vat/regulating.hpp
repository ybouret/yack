
//! \file

#ifndef YACK_CHEM_VAT_REGULATING_INCLUDED
#define YACK_CHEM_VAT_REGULATING_INCLUDED 1

#include "yack/chem/vat/limit.hpp"


namespace yack
{
    namespace Chemical
    {



        // Regulation: Limiting with selected extent
        class Regulating : public Limit
        {
        public:
            explicit Regulating(const Species::Fund &) noexcept;
            virtual ~Regulating() noexcept;

            void     tryDecreaseWith(const double c, const Actor &a);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Regulating);
        };

    }

}

#endif
