
//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/eqs/actor.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! base class for Actors
        //
        //______________________________________________________________________
        typedef cxx_list_of<Actor> Actors_;

        //______________________________________________________________________
        //
        //
        //! List of Actors with a naming system
        //
        //______________________________________________________________________
        class Actors :  public Actors_, public Entity
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Actors();          //!< setup empty
            Actors(const Actors &);     //!< copy
            virtual ~Actors() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            YACK_PROTO_OSTREAM(Actors); //!< display
            void makeName();            //!< create name from list

            //! Prod C_i^nu_i
            void massAction(Extended::Mul                  &xmul,
                            const readable<Extended::Real> &C,
                            const IndexLevel                level) const;

            //! nu_i C_i^(nu_i-1) * Prod_{j!=i} C_j^nu_j
            void gradAction(writable<Extended::Real>       &out,
                            const IndexLevel                outLevel,
                            const Extended::Real           &fac,
                            const readable<Extended::Real> &inp,
                            const IndexLevel                inpLevel,
                            Extended::Mul                  &xmul) const;

        private:
            YACK_DISABLE_ASSIGN(Actors);
        };

    }

}

#endif

