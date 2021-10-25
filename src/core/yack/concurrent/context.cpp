
#include "yack/concurrent/context.hpp"
#include "yack/arith/base10.hpp"
#include "yack/type/cstring.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>

namespace yack
{
    namespace concurrent
    {

        context:: context() throw() : size(1), rank(0), indx(1), io10(1) {}

        context:: ~context() throw()
        {
            coerce(size) = 0;
            coerce(rank) = 0;
            coerce(indx) = 0;
        }

        context:: context(const size_t sz, const size_t rk) throw() :
        size(sz),
        rank(rk),
        indx(1+rank),
        io10(digits_for(size))
        {
            assert(size>0);
            assert(rank<size);
        }

        
        std::ostream & operator<<(std::ostream &os,const context &ctx)
        {
            char buf[32];
            ctx.format(buf,sizeof(buf));
            os << buf;
            return os;
        }

        void context:: format(char buf[], const size_t len) const throw()
        {
            assert(buf!=NULL);
            assert(len>0);

            char fmt[32];
            {
                memset(fmt,0,sizeof(fmt));
                snprintf(fmt,sizeof(fmt),"%%0%uu.%%0%uu",unsigned(io10),unsigned(io10));
            }

            memset(buf,0,len);
            snprintf(buf,len,fmt,unsigned(size),unsigned(indx));
        }


    }

}

