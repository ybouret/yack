//! \file

#ifndef YACK_CHEM_ACTOR_INCLUDED
#define YACK_CHEM_ACTOR_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {

        class actor : public object
        {
        public:
            explicit actor(const species &sp, const unsigned cf) throw();
            virtual ~actor() throw();

            actor          *next;
            actor          *prev;
            const  unsigned coef; //!< |nu|>0
            const  unsigned cder; //!< coef-1 >= 0

            const species & operator*() const throw();

            double limiting(const readable<double> &C) const throw();

            double mass_action(const readable<double> &C) const throw();
            double drvs_action(const readable<double> &C) const throw();


        private:
            const species &host;
            YACK_DISABLE_COPY_AND_ASSIGN(actor);
        };

    }

}
#endif
