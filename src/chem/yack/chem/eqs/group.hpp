//! \file


#ifndef YACK_CHEM_EQS_GROUP_INCLUDED
#define YACK_CHEM_EQS_GROUP_INCLUDED 1


#include "yack/chem/equilibrium.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    namespace chemical
    {
        typedef meta_list<const equilibrium> group_type;
        typedef group_type::node_type        gnode;

        
        class group : public object, public group_type
        {
        public:
            explicit group() throw();
            virtual ~group() throw();

            group *next; //!< for groups
            group *prev; //!< for groups

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(group);
        };


    }

}

#endif

