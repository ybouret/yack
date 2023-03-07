//! \file

#ifndef YACK_AQUEOUS_ACTORS_INCLUDED
#define YACK_AQUEOUS_ACTORS_INCLUDED 1

#include "yack/aqueous/actor.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace aqueous
    {
        typedef cxx_list_of<actor> actors_;

        class actors : public actors_
        {
        public:
            explicit actors() noexcept;
            virtual ~actors() noexcept;
            actors(const actors &);

            YACK_PROTO_OSTREAM(actors);
            
        private:
            YACK_DISABLE_ASSIGN(actors);
        };

    }

}

#endif

