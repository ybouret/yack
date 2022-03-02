//! \file

#ifndef YACK_CHEM_ACTOR_INCLUDED
#define YACK_CHEM_ACTOR_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! an actor is a species with a positive coefficient
        //
        //______________________________________________________________________
        class actor : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit actor(const species &sp, const unsigned cf) throw();

            //! cleanup
            virtual ~actor() throw();


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const species & operator*() const throw(); //!< access species

            //! return limiting extent
            double limiting_extent(const readable<double> &C) const throw();

            //! C[indx]^coef
            double mass_action(const readable<double> &C) const throw();

            //! (C[indx]+coef*xi)^coef
            double mass_action(const readable<double> &C, const double xi) const throw();

            //! C[indx] += xi * coef;
            void move(writable<double> &C, const double xi) const throw();


            //! coef*C[indx]^cder
            double drvs_action(const readable<double> &C) const throw();

            //! helper to display
            void    display(std::ostream &os, const bool first) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            actor          *next; //!< for actors
            actor          *prev; //!< for actors
            const  unsigned coef; //!< |nu|>0
            const  unsigned cder; //!< coef-1 >= 0

        private:
            const species &host;
            YACK_DISABLE_COPY_AND_ASSIGN(actor);
        };

    }

}
#endif
