
//! \file

#ifndef YACK_CHEMICAL_BALANCING_INCLUDED
#define YACK_CHEMICAL_BALANCING_INCLUDED 1

#include "yack/chem/nexus.hpp"
#include "yack/type/authority.hpp"


namespace yack
{

    namespace chemical
    {


        class balancing : public authority<const nexus>
        {
        public:
            explicit balancing(const nexus &);
            virtual ~balancing() throw();
            


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(balancing);
        };


    }

}

#endif

