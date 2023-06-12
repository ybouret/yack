//! \file

#ifndef YACK_CHEM_AFTERMATH_INCLUDED
#define YACK_CHEM_AFTERMATH_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/chem/eqs/extents.hpp"

namespace yack
{
    namespace Chemical
    {

        enum EqSatus
        {
            Blocked,
            Running
        };

        class Aftermath
        {
        public:
            explicit Aftermath()                       noexcept;
            explicit Aftermath(const Extended::Real &) noexcept;
            Aftermath(const Aftermath &)               noexcept;
            virtual ~Aftermath()                       noexcept;

            const Extended::Real extent;
            const EqSatus        status;

            static Aftermath Evaluate(const Equilibrium              &eq,
                                      const Extended::Real           &K,
                                      const writable<Extended::Real> &Cend,
                                      const readable<Extended::Real> &Corg,
                                      Extents                        &extents,
                                      const IndexLevel                level);


        private:
            YACK_DISABLE_ASSIGN(Aftermath);
        };

    }

}

#endif

