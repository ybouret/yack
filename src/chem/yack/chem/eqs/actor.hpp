//! \file

#ifndef YACK_CHEM_ACTOR_INCLUDED
#define YACK_CHEM_ACTOR_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace Chemical
    {


        class Actor : public object
        {
        public:
            explicit Actor(const unsigned, const Species &) noexcept;
            virtual ~Actor() noexcept;

            const Species & operator*() const noexcept;

            const unsigned nu;

            void   displayFirst(std::ostream &) const;
            void   displayExtra(std::ostream &) const;
            string displayFirst() const;
            string displayExtra() const;


        private:
            const Species &sp;
            YACK_DISABLE_ASSIGN(Actor);
        public:
            Actor *next;
            Actor *prev;
        };

    }

}

#endif

