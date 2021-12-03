
//! \file

#ifndef YACK_JIVE_PATTERN_LOGICAL_BANK_INCLUDED
#define YACK_JIVE_PATTERN_LOGICAL_BANK_INCLUDED 1

#include "yack/jive/pattern/logic/logical.hpp"

namespace yack
{
    namespace jive
    {

        class bank : public logical
        {
        public:
            virtual ~bank() throw();
            void     no_multiple() throw();


        protected:
            explicit bank(const uint32_t) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bank);
        };

    }

}

#endif
