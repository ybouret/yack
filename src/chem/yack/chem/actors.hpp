
//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! base class for actors
        //
        //______________________________________________________________________
        typedef cxx_list_of<actor> actors_;


        //______________________________________________________________________
        //
        //
        //! list of actors
        //
        //______________________________________________________________________
        class actors : public actors_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actors() throw(); //!< setup
            virtual ~actors() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! factor * prod( C[indx]^coef[indx] )
            double mass_action(double factor, const readable<double> &C) const throw();

            int    nu() const throw(); //!< sum of each nu, cast to int
            int    dz() const throw(); //!< sum of algebraic nu

            //! helper to display
            void display(std::ostream &) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
        };
    }
}

#endif



