//! \file

#ifndef YACK_DATA_STATS_INCLUDED
#define YACK_DATA_STATS_INCLUDED 1

#include "yack/sequence/vector.hpp"
#include "yack/sort/sum.hpp"

namespace yack
{
    
    namespace statistical
    {
        template <typename T>
        struct average
        {
            template <typename ITERATOR> static
            inline T of_(ITERATOR it, const size_t n)
            {
                assert(n>0);
                T      sum = *(it++);
                size_t idx = n;
                while(--idx > 0)
                {
                    sum += *(it++);
                }
                return sum/n;
            }
            
            template <typename ITERATOR, typename SEQUENCE> static
            inline T of_(ITERATOR it, const size_t n,SEQUENCE &temp)
            {
                assert(n>0);
                temp.free();
                for(size_t i=n;i>0;--i)
                {
                    temp.push_back( *(it++) );
                }
                return sorted::sum(temp,sorted::by_abs_value)/n;
            }
            
        };
        
        
    }
}

#endif

