//! \file

#ifndef YACK_CHEM_AFTERMATH_INCLUDED
#define YACK_CHEM_AFTERMATH_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/chem/eqs/extents.hpp"

namespace yack
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //! 1D equilibrium solotuin
        //
        //______________________________________________________________________
        class Aftermath
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Aftermath()                        noexcept; //!< 0,blocked,0
            explicit Aftermath(const Extended::Real xi) noexcept; //!< xi,running,0
            explicit Aftermath(const Extended::Real xi,
                               const Extended::Real ma) noexcept; //!< xi, running, max
            Aftermath(const Aftermath &)               noexcept; //!< copy
            virtual ~Aftermath()                       noexcept; //!< cleanup




            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            YACK_PROTO_OSTREAM(Aftermath); //!< display

            //! Evaluate 1D solution
            /**
             \param eq      current equilibrium
             \param K       current constant
             \param Cend    computed solution
             \param Corg    original concentrations
             \param extents evaluates limiting species
             \param level   where to evaluate concentrations
             \param xmul    for mass actions
             \param xadd    for extent evaluation
             \param Ctmp    workspace for concentrations
             */
            static Aftermath Evaluate(const Equilibrium              &eq,
                                      const Extended::Real           &K,
                                      writable<Extended::Real>       &Cend,
                                      const readable<Extended::Real> &Corg,
                                      Extents                        &extents,
                                      const IndexLevel                level,
                                      Extended::Mul                  &xmul,
                                      Extended::Add                  &xadd,
                                      writable<Extended::Real>       &Ctmp);
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Extended::Real      extent; //!< extent from Corg to Cend
            const Equilibrium::Status status; //!< blocked/running
            const Extended::Real      action; //!< action at Cend

        private:
            YACK_DISABLE_ASSIGN(Aftermath);
            static Extended::Real makeExtent(const Components               &eq,
                                             const readable<Extended::Real> &Corg,
                                             const readable<Extended::Real> &Cend,
                                             const IndexLevel                level,
                                             Extended::Add                  &xadd);

            static bool QueryLimitedByBoth(Extended::Triplet              &xi,
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

