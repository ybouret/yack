
//! \file

#ifndef YACK_CHEMICAL_ACTIVE_INCLUDED
#define YACK_CHEMICAL_ACTIVE_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    namespace chemical
    {

        typedef meta_list<const species> alist;
        typedef alist::node_type         anode;

        class active_list : public alist
        {
        public:
            explicit active_list() throw();
            virtual ~active_list() throw();
            explicit active_list(const snode *);

            template <typename TARGET, typename SOURCE> inline
            void tranfer(TARGET &target, SOURCE &source) const
            {
                for(const anode *node=this->head;node;node=node->next)
                {
                    const size_t j = ***node;
                    target[j] = source[j];
                }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(active_list);
        };
        
    }

}

#endif
