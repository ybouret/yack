
//! \file

#ifndef YACK_AQUEOUS_ACTOR_INCLUDED
#define YACK_AQUEOUS_ACTOR_INCLUDED 1

#include "yack/aqueous/species.hpp"

namespace yack
{
    namespace aqueous
    {

        class actor : public object
        {
        public:
            explicit actor(const species &, const unsigned) noexcept;
            virtual ~actor()                                noexcept;
            actor(const actor &)                            noexcept;
            const species & operator*() const noexcept;


            const unsigned nu; //!< nu>0
            const unsigned nm; //!< nu-1

        private:
            const species  &sp;
            YACK_DISABLE_ASSIGN(actor);

        public:
            actor *next;
            actor *prev;
        };

    }

}

#endif

