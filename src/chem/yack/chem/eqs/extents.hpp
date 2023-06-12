//! \file

#ifndef YACK_CHEM_EXTENTS_INCLUDED
#define YACK_CHEM_EXTENTS_INCLUDED 1

#include "yack/chem/eqs/regulating.hpp"
#include "yack/chem/eqs/components.hpp"

namespace yack
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! possible limitations
        //
        //______________________________________________________________________
        enum Limitation
        {
            LimitedByNone, //!< no limitation
            LimitedByReac, //!< limited by reactant(s) only
            LimitedByProd, //!< limited by product(s)  only
            LimitedByBoth  //!< limited by both
        };

        //! convert Limitation to text
        const char *LimitationText(const Limitation) noexcept;

        //______________________________________________________________________
        //
        //
        //! limiting extents for an equilibrium and a set of concentrations
        //
        //______________________________________________________________________
        class Extents
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Extents(const Species::Fund &) noexcept; //!< setup
            virtual ~Extents() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void reset() noexcept; //!< clean all

            //! build current limitation
            void build(const Components               &eq,
                       const readable<Extended::Real> &C0,
                       const IndexLevel                level);

            //! output status
            std::ostream & display(std::ostream &os) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
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

