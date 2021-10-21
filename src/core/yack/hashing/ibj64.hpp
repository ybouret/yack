

//! \file

#ifndef YACK_HASH64_IBJ_INCLUDED
#define YACK_HASH64_IBJC_INCLUDED 1

#include "yack/hashing/hash64.hpp"

namespace yack
{

    namespace hashing
    {

        uint32_t ibj32(uint32_t) throw();

        class ibj64 : public hash64
        {
        public:
            static const char clid[];

            explicit ibj64() throw();
            virtual ~ibj64() throw();

            virtual const char *protocol()                                   const throw();
            virtual void        operator()(uint32_t &lword, uint32_t &rword) const throw();

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ibj64);
        };

    }
}

#endif

