
//! \file

#ifndef YACK_HASH64_DES_INCLUDED
#define YACK_HASH64_DES_INCLUDED 1

#include "yack/hashing/hash64.hpp"

namespace yack
{

    namespace hashing
    {

        class des64 : public hash64
        {
        public:
            static const char clid[];

            explicit des64() throw();
            virtual ~des64() throw();

            virtual const char *protocol()                                   const throw();
            virtual void        operator()(uint32_t &lword, uint32_t &rword) const throw();

            bool test() const throw();

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(des64);
        };

    }
}

#endif

