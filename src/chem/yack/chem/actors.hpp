//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/eq/limiting.hpp"

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
            //! K * partial law of mass action
            double          mass_action(double K, const readable<double> &C) const throw();

            //! K * derivatives
            void            drvs_action(writable<double> &psi, const double K, const readable<double> &C) const throw();

            //! maximum
            double          maximum(const readable<double> &C) const throw();


            //! limiting species, associated extent
            const limiting *find_limiting(const readable<double> &C) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
            mutable void *wksp[YACK_WORDS_FOR(limiting)];

            const limiting *make(const actor *pa, const double xi) const throw();
        };

        
    }

}

#endif

