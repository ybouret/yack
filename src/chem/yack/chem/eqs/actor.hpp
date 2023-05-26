//! \file

#ifndef YACK_CHEM_ACTOR_INCLUDED
#define YACK_CHEM_ACTOR_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! an Actor is a species with a POSITIVE coefficient
        //
        //______________________________________________________________________
        class Actor : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Actor(const unsigned, const Species &) noexcept; //!< setup
            Actor(const Actor &)                            noexcept; //!< copy
            virtual ~Actor()                                noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const Species & operator*()         const noexcept; //!< access
            void            displayFirst(std::ostream &) const; //!< display
            void            displayExtra(std::ostream &) const; //!< display
            string          displayFirst()               const; //!< format
            string          displayExtra()               const; //!< format

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned nu; //!< coefficient>0

        private:
            const Species &sp;
            YACK_DISABLE_ASSIGN(Actor);
        public:
            Actor *next; //!< for list
            Actor *prev; //!< for list
        };

    }

}

#endif

