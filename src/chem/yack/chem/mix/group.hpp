
//! \file

#ifndef YACK_CHEM_MIX_GROUP_INCLUDED
#define YACK_CHEM_MIX_GROUP_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        // global definitions
        //______________________________________________________________________
        typedef meta_list<const equilibrium> glist; //!< alias
        typedef glist::node_type             gnode; //!< alias

        //______________________________________________________________________
        //
        //
        //! group of equilibria
        //
        //______________________________________________________________________
        class group : public object, public glist
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef cxx_list_of<group> list; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~group() throw(); //!< cleanup
            explicit group() throw(); //!< setup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void sort();                                                           //!< sort in ascending index order
            bool is_valid() const throw();                                         //!< was sorted
            bool is_ortho() const throw();                                         //!< each equilibrium is detached from the others
            bool contains(const equilibrium &) const throw();                      //!< check if equilibrium is inside list
            bool contains(const equilibrium &, const equilibrium &) const throw(); //!< contains two different equilibria
            bool contains(const group &)       const throw();                      //!< check if all equilibria are inside list
            bool attached(const equilibrium &) const throw();                      //!< check is equilibrium is attached to one of the list
            bool detached(const equilibrium &) const throw();                      //!< check is equilibrium is detached from all of the list

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static int compare_(const gnode *lhs, const gnode *rhs);         //!< compare two DIFFERENT gnodes
            static int compare(const group *lhs, const group *rhs);          //!< comparing two SORTED, DIFFERENT  groups, size+lexico
            friend std::ostream & operator<<(std::ostream &, const group &); //!< output
            static group *start_from(const equilibrium &eq);                 //!< create a new group with this equilibrium

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            group *next; //!< for list
            group *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(group);
        };

    }

}


#endif
