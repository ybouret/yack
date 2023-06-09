//! \file

#ifndef YACK_CHEM_EXTENTS_INCLUDED
#define YACK_CHEM_EXTENTS_INCLUDED 1

#include "yack/chem/eqs/regulating.hpp"

namespace yack
{
    namespace Chemical
    {

        enum Limitation
        {
            LimitedByNone,
            LimitedByReac,
            LimitedByProd,
            LimitedByBoth
        };

        class Extents
        {
        public:
            explicit Extents(const Species::Fund &) noexcept;
            virtual ~Extents() noexcept;


            void reset() noexcept;
            


            const Regulating reac;
            const Regulating prod;
            const Limitation kind;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Extents);
        };

    }

}

#endif

