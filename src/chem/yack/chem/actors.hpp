
//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace chemical
    {

        typedef cxx_list_of<actor> actors_;

        class actors : public actors_
        {
        public:
            explicit actors() throw();
            virtual ~actors() throw();

            double mass_action(double factor, const readable<double> &C) const throw();
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
        };
    }
}

#endif



