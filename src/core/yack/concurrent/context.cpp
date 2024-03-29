
#include "yack/concurrent/context.hpp"
#include "yack/arith/base10.hpp"
#include "yack/type/cstring.h"
#include "yack/container/groove.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>

namespace yack
{
    namespace concurrent
    {

        context:: context() noexcept : size(1), rank(0), indx(1), data(0) {}

        context:: ~context() noexcept
        {
            coerce(size) = 0;
            coerce(rank) = 0;
            coerce(indx) = 0;
            if(data)
            {
                delete data;
                data = NULL;
            }
        }

        context:: context(const size_t sz, const size_t rk) noexcept :
        size(sz),
        rank(rk),
        indx(1+rank),
        data(0)
        {
            assert(size>0);
            assert(rank<size);
            assert(indx>0);
            assert(indx<=size);
        }

        
        std::ostream & operator<<(std::ostream &os,const context &ctx)
        {
            char buf[32];
            ctx.format(buf,sizeof(buf));
            os << buf;
            return os;
        }

        void context:: format(char         buf[],
                              const size_t len, const size_t size, const size_t indx)   noexcept
        {
            assert(buf!=NULL);
            assert(len>0);
            assert(size>0);
            assert(indx>0);
            assert(indx<=size);
            const size_t io10 = digits_for(size);
            char fmt[32];
            {
                memset(fmt,0,sizeof(fmt));
                snprintf(fmt,sizeof(fmt),"%%0%uu.%%0%uu",unsigned(io10),unsigned(io10));
            }
            
            memset(buf,0,len);
            snprintf(buf,len,fmt,unsigned(size),unsigned(indx));
        }
        
        void context:: format(char buf[], const size_t len) const noexcept
        {
            format(buf,len,size,indx);
        }

        groove & context:: operator*() const
        {
            return data ? *data : *(data=new groove());
        }

        groove *  context:: operator->() const
        {
            return & (**this);
        }

        bool context:: owns_local_memory() const noexcept
        {
            return NULL != data;
        }

        size_t context:: local_memory_size() const noexcept
        {
            if(data)
            {
                return data->granted();
            }
            else
            {
                return 0;
            }
        }

        void context:: drop_local_memory() noexcept
        {
            if(data)
            {
                delete data;
                data = NULL;
            }
        }

        void context:: free_local_memory() noexcept
        {
            if(data)
            {
                data->free();
            }
        }


    }

}

