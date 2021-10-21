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

            template <typename T> inline
            T mix(const T value) const throw()
            {
                union {
                    uint32_t dw[2];
                    T        xx;
                } alias = { {0,0} };
                alias.xx = value;
                (*this)(alias.dw[0],alias.dw[1]);
                return alias.xx;
            }

        protected:
            explicit hash64() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hash64);
        };

    }
}

#endif

