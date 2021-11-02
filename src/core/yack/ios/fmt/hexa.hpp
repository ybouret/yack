//! \file

#ifndef YACK_IOS_FMT_HEXA_INCLUDED
#define YACK_IOS_FMT_HEXA_INCLUDED 1

#include "yack/type/ints.hpp"

namespace yack
{

    namespace ios
    {


        class hexa
        {
        public:
            template <typename T>
            inline hexa(const T x) throw() :
            qword( uint64_t(x) ),
            count(width(qword))
            {
            }

            hexa(const hexa &_) throw();
            ~hexa() throw();

            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const hexa &x)
            {
                char tab[16];
                size_t n = x.load(tab); assert(n>0);
                while(n-- > 0) os << tab[n];
                return os;
            }

        private:
            YACK_DISABLE_ASSIGN(hexa);
            const uint64_t qword;
            const size_t   count;
            size_t         load(char tab[]) const;
            static size_t  width(const uint64_t) throw();

        };


    }
}


#endif

