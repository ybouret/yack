
//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/limit.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/arith/align.hpp"

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
            int    nu() const throw(); //!< sum of each nu, cast to int
            int    dz() const throw(); //!< sum of algebraic nu

            //! factor * prod( C[indx]^coef[indx] )
            double mass_action(double factor, const readable<double> &C) const throw();

            //! factor * prod( (C[indx]+coef*xi)^coef[indx] )
            double mass_action(double factor, const readable<double> &C, const double xi) const throw();

            //! move all actors
            void   move(writable<double> &C, const double xi) const throw();

            //! maximum of involved
            double maximum(const readable<double> &C) const throw();

            //! find limit from all actors
            const limit *private_limit(const readable<double> &C) const throw();

            //! helper to display
            void display(std::ostream &) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
            mutable void *wksp[ YACK_WORDS_FOR(limit) ];
        };
    }
}

#endif



