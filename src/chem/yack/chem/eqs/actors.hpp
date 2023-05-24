
//! \file

#ifndef YACK_CHEM_ACTORS_INCLUDED
#define YACK_CHEM_ACTORS_INCLUDED 1

#include "yack/chem/eqs/actor.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace Chemical
    {

        typedef cxx_list_of<Actor> Actors_;

        class Actors : public Actors_, public Entity
        {
        public:
            explicit Actors();
            virtual ~Actors() noexcept;

            friend std::ostream & operator<<(std::ostream &, const Actors &);

            void   makeName();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Actors);
        };

    }

}

#endif

