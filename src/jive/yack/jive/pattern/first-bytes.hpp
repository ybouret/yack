//! \file

#ifndef YACK_JIVE_PATTERN_FIRST_BYTES_INCLUDED
#define YACK_JIVE_PATTERN_FIRST_BYTES_INCLUDED 1

#include "yack/jive/pattern/hood/partition.hpp"

namespace yack
{
    namespace jive
    {

        class first_bytes : public hood::partition
        {
        public:
            virtual ~first_bytes() throw();
            explicit first_bytes() throw();

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(first_bytes);

        };
    }
}

#endif

