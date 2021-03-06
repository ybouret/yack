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
            // types and definitions
            //__________________________________________________________________

            //! family of actors, to compute topologies
            enum family
            {
                empty    = 0x00,  //!< empty
                kind_1   = 0x01,  //!< 1 * [A]
                kind_11  = 0x02,  //!< 1 * [A] + 1 * [B]
                kind_2   = 0x04,  //!< 2 * [A]
                kind_n   = 0x08,  //!< n * [A]
                kind_xx  = 0x10   //!< other
            };

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

            //! K * derivatives, return mass action
            double         drvs_action(writable<double> &psi, const double K, const readable<double> &C) const throw();

            //! maximum
            double          maximum(const readable<double> &C) const throw();

            //! max species index
            size_t          span() const throw();  

            //! limiting species, associated extent for self
            const limiting *find_private_limiting(const readable<double> &C) const throw();

            //! limiting primary species, associated extent
            const limiting *find_primary_limiting(const readable<double> &C) const throw();

            //! get family
            family get_family() const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
            mutable void *wksp[YACK_WORDS_FOR(limiting)];

            const limiting *make(const actor *pa, const double xi) const throw();
        };

        
    }

}

#endif

