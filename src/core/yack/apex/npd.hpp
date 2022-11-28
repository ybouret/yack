
//! \file

#ifndef YACK_APEX_NPD_INCLUDED
#define YACK_APEX_NPD_INCLUDED 1

#include "yack/apex/natural.hpp"
#include "yack/ptr/shared.hpp"

namespace yack
{
    namespace apex
    {

        typedef shared_ptr<const natural> shared_natural;
        

        class npd
        {
        public:

            virtual ~npd() throw();
            
        private:

        };
    }
}


#endif

