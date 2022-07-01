
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

        class group : public glist
        {
        public:
            virtual ~group() throw();
            explicit group() throw();
            void     sort();
            static int compare_nodes(const gnode *lhs, const gnode *rhs);
            friend std::ostream & operator<<(std::ostream &, const group &);


            bool contains(const equilibrium &) const throw(); //!< check if equilibrium is inside list
            bool contains(const group &)       const throw(); //!< check if all equilibria are inside list

            bool attached(const equilibrium &) const throw(); //!< check is equilibrium is attached to one of the list
            bool detached(const equilibrium &) const throw(); //!< check is equilibrium is detached from all of the list

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(group);
        };

    }

}


#endif
