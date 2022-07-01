
//! \file

#ifndef YACK_CHEM_MIX_GROUP_INCLUDED
#define YACK_CHEM_MIX_GROUP_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    namespace chemical
    {

        typedef meta_list<const equilibrium> glist;
        typedef glist::node_type             gnode;

        class group : public object, public glist
        {
        public:
            typedef cxx_list_of<group> list;
            virtual ~group() throw();
            explicit group() throw();
            void     sort();

            bool is_valid() const throw();                    //!< was sorted
            bool contains(const equilibrium &) const throw(); //!< check if equilibrium is inside list
            bool contains(const group &)       const throw(); //!< check if all equilibria are inside list

            bool attached(const equilibrium &) const throw(); //!< check is equilibrium is attached to one of the list
            bool detached(const equilibrium &) const throw(); //!< check is equilibrium is detached from all of the list



            static int compare_(const gnode *lhs, const gnode *rhs); //!< compare two DIFFERENT gnodes
            static int compare(const group *lhs, const group *rhs) throw(); //!< comparing two SORTED  groups
            friend std::ostream & operator<<(std::ostream &, const group &);
            

            group *next;
            group *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(group);
        };

    }

}


#endif
