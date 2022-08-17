

#ifndef YACK_CHEM_EQS_GROUPS_INCLUDED
#define YACK_CHEM_EQS_GROUPS_INCLUDED 1


#include "yack/chem/eqs/group.hpp"

namespace yack
{
    namespace chemical
    {


        class groups :  public cxx_list_of<group>
        {
        public:
            explicit groups() throw();
            virtual ~groups() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(groups);
        };


    }

}

#endif

