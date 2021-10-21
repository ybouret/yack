//! \file

#ifndef YACK_HASH64_INCLUDED
#define YACK_HASH64_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace hashing
    {

        class hash64
        {
        public:
            virtual ~hash64() throw();

            virtual const char *protocol()                                   const throw() = 0;
            virtual void        operator()(uint32_t &lword, uint32_t &rword) const throw() = 0;

        protected:
            explicit hash64() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hash64);
        };

    }
}

#endif

