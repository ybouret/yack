
//! \file

#ifndef YACK_CHEM_VAT_LEAVEN_INCLUDED
#define YACK_CHEM_VAT_LEAVEN_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack
{
    namespace Chemical
    {

        class Leaven
        {
        public:
            explicit Leaven();
            virtual ~Leaven() noexcept;

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Leaven);
        };

    }

}

#endif
