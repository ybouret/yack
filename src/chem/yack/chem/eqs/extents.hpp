//! \file

#ifndef YACK_CHEM_EXTENTS_INCLUDED
#define YACK_CHEM_EXTENTS_INCLUDED 1

#include "yack/chem/eqs/regulating.hpp"
#include "yack/chem/eqs/components.hpp"

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

        const char *LimitationText(const Limitation) noexcept;

        class Extents
        {
        public:
            explicit Extents(const Species::Fund &) noexcept;
            virtual ~Extents() noexcept;


            void reset() noexcept;
            void build(const Components               &eq,
                       const readable<Extended::Real> &C0,
                       const IndexLevel                level);

            std::ostream & display(std::ostream &os) const;

            const Regulating reac; //!< regulating reactant(s)
            const Regulating prod; //!< regulating product(s)
            const Limitation kind; //!< depending on counts

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Extents);
            void updateLimitation() noexcept;
        };

    }

}

#endif

