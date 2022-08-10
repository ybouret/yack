//! \file

#ifndef YACK_CHEMICAL_ACTOR_INCLUDED
#define YACK_CHEMICAL_ACTOR_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! single actor
        //
        //______________________________________________________________________
        class actor : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~actor() throw();
            
            //! setup from persistent species and positive coefficient
            explicit actor(const species &sr,
                           const unsigned cf) throw();

            //__________________________________________________________________
            //
            // method
            //__________________________________________________________________
           
            //! access species
            const species & operator*() const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            actor *next; //!< for list
            actor *prev; //!< for list

        private:
            const species &sp; //!< persisent species

        public:
            const unsigned nu; //!< nu>0
            const unsigned nm; //!< nu-1

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actor);
        };

    }

}
#endif

