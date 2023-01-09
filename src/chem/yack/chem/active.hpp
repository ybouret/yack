
//! \file

#ifndef YACK_CHEMICAL_ACTIVE_INCLUDED
#define YACK_CHEMICAL_ACTIVE_INCLUDED 1

#include "yack/chem/sub-list.hpp"
#include "yack/chem/library.hpp"

namespace yack
{
    namespace chemical
    {

        typedef sub_list<species> alist;
        typedef sub_node<species> anode;

        class active_list : public object, public alist
        {
        public:
            explicit active_list() throw();
            virtual ~active_list() throw();
            explicit active_list(const snode *);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(active_list);
        };



    }

}

#endif

