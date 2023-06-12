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
            explicit Aftermath()                     noexcept;
            explicit Aftermath(const Extended::Real) noexcept;
            explicit Aftermath(const Extended::Real,
                               const Extended::Real) noexcept;

            Aftermath(const Aftermath &)               noexcept;
            virtual ~Aftermath()                       noexcept;

            const Extended::Real extent;
            const EqSatus        status;
            const Extended::Real action;

            static Aftermath Evaluate(const Equilibrium              &eq,
                                      const Extended::Real           &K,
                                      writable<Extended::Real>       &Cend,
                                      const readable<Extended::Real> &Corg,
                                      Extents                        &extents,
                                      const IndexLevel                level,
                                      Extended::Mul                  &xmul,
                                      Extended::Add                  &xadd,
                                      writable<Extended::Real>       &Ctmp);


        private:
            YACK_DISABLE_ASSIGN(Aftermath);
            static Extended::Real makeExtent(const Components               &eq,
                                             const readable<Extended::Real> &Corg,
                                             const readable<Extended::Real> &Cend,
                                             const IndexLevel                level,
                                             Extended::Add                  &xadd);

            static bool InitBoth(Extended::Triplet              &xi,
                                 Extended::Triplet              &ma,
                                 const Equilibrium              &eq,
                                 const Extended::Real           &K,
                                 writable<Extended::Real>       &Ctmp,
                                 const readable<Extended::Real> &Cend,
                                 const Extents                  &extents,
                                 const IndexLevel                level,
                                 Extended::Mul                  &xmul);

        };

    }

}

#endif

