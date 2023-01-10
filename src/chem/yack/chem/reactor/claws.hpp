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
        class claw : public object, public indexed
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claw(const size_t i) throw(); //!< setup
            virtual ~claw() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const list_of<actor> * operator->() const throw(); //!< access
            void add(const species  &, const unsigned);        //!< add a new species

            //! specific display
            friend std::ostream & operator<<(std::ostream &, const claw &);

            //! maximum of species index
            size_t span() const throw();

            bool contains(const species &s) const throw() {
                for(const actor *a=crew.head;a;a=a->next)
                {
                    if( &s == & **a ) return true;
                }
                return false;
            }

            bool attached_to(const claw &other) const throw()
            {
                for(const actor *a=other->head;a;a=a->next)
                {
                    if(contains(**a)) return true;
                }
                return false;
            }

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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t span() const throw(); //!< max of spans

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claws);
        };

    }
}

#endif

