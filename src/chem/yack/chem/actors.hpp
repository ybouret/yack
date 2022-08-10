
//! \file

#ifndef YACK_CHEMICAL_ACTORS_INCLUDED
#define YACK_CHEMICAL_ACTORS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/math/multiplier.hpp"

namespace yack
{
    namespace chemical
    {

        typedef math::multiplier<double> rmulops;

        class actors
        {
        public:
            virtual ~actors() throw();
            explicit actors() throw();

            void operator()(const species &sp,
                            const unsigned nu);


            void remove_last() throw();

            double mass_action(const readable<double> &C,
                               rmulops                &ops) const;


            const size_t       molecularity;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actors);
            cxx_list_of<actor> crew;
        };

    }

}

#endif
