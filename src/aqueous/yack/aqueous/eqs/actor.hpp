//! \file

#ifndef YACK_AQUEOUS_ACTOR_INCLUDED
#define YACK_AQUEOUS_ACTOR_INCLUDED 1

#include "yack/aqueous/species.hpp"
#include "yack/cameo/mul.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! an actor is a species with a POSITIVE stoichiometric coefficient
        //
        //______________________________________________________________________
        class actor : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actor(const species &, const unsigned) noexcept; //!< setup from persistent species
            virtual ~actor()                                noexcept; //!< cleanup
            actor(const actor &)                            noexcept; //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const species & operator*() const noexcept; //!< access species

            void   display_first(std::ostream &) const; //!< display as first actor
            void   display_extra(std::ostream &) const; //!< display as extra actor
            string to_string_first()             const; //!< stringify as first actor
            string to_string_extra()             const; //!< stringify as extra actor

            //! load xmul with mass action
            void   mass_action(const readable<double> &C,
                               const index_level       I,
                               cameo::mul<double>     &xmul) const;

            //! load xmul with grad action, return species index
            size_t  grad_action(const readable<double> &C,
                                const index_level       I,
                                cameo::mul<double>     &xmul) const;
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unsigned nu; //!< nu>0, stoichiometric coefficient
            const unsigned nm; //!< nu-1, for derivative of mass action

        private:
            const species  &sp;
            YACK_DISABLE_ASSIGN(actor);

        public:
            actor *next; //!< for list
            actor *prev; //!< for list
        };

    }

}

#endif

