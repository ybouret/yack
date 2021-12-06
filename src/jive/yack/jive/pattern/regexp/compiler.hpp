
//! \file

#ifndef YACK_JIVE_REGEXP_INCLUDED
#define YACK_JIVE_REGEXP_INCLUDED 1

#include "yack/jive/pattern.hpp"

namespace yack
{
    namespace jive
    {

        //! internal rx_compiler
        class rx_compiler
        {
        public:
            virtual ~rx_compiler() throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rx_compiler);
        };
    }

}

#endif
