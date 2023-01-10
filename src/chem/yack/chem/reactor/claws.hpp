//! \file

#ifndef YACK_CHEMICAL_CLAWS_INCLUDED
#define YACK_CHEMICAL_CLAWS_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/data/dinky/core-repo.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! conservation law: more than two species
        //
        //______________________________________________________________________
        class claw : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claw() throw(); //!< setup
            virtual ~claw() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const list_of<actor> & operator *() const throw(); //!< access
            const list_of<actor> * operator->() const throw(); //!< access
            void add(const species  &, const unsigned);        //!< add a new species

            //! specific display
            friend std::ostream & operator<<(std::ostream &, const claw &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            claw *next; //!< for list
            claw *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw);
            cxx_list_of<actor> crew;
        };

        //______________________________________________________________________
        //
        //
        //! base class for conservation laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<claw> claws_;

        //______________________________________________________________________
        //
        //
        //! list of conservations laws
        //
        //______________________________________________________________________
        class claws : public object, public claws_
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const claws>  ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claws() throw(); //!< setup
            virtual ~claws() throw(); //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claws);
        };

    }
}

#endif

