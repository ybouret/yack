//! \file

#ifndef YACK_DATA_STATS_INCLUDED
#define YACK_DATA_STATS_INCLUDED 1

#include "yack/sequence/vector.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include "yack/math/adder.hpp"
#include <cmath>

namespace yack
{
    
    namespace statistical
    {
        //______________________________________________________________________
        //
        //! sample average
        //______________________________________________________________________
        template <typename T> struct average
        {
            //! from iterator
            template <typename ITERATOR> static
            inline T of(ITERATOR it, const size_t n, math::adder<T> &xadd)
            {
                assert(n>0);
                return xadd.range(it,n)/n;
            }


            //! for a whole sequence
            template <typename SEQUENCE> static
            inline T of(SEQUENCE &seq, math::adder<T> &xadd)
            {
                return of( seq.begin(), seq.size(), xadd);
            }

        };


        //______________________________________________________________________
        //
        //! sample variance
        //______________________________________________________________________
        template <typename T>
        struct variance
        {
            
            //! from iterator with sorted sum
            template <typename ITERATOR> static inline
            T of(ITERATOR it, const size_t n, const T ave,  math::adder<T> &xadd)
            {
                assert(n>1);
                xadd.resume(n);
                for(size_t i=n;i>0;--i)
                {
                    const T  del = *(it++) - ave;
                    const T  sqr = del*del;
                    xadd.ld(sqr);
                }
                return xadd.get() / (n-1);
            }


            //! for a whole sequence
            template <typename SEQUENCE> static
            inline T of(SEQUENCE &seq, const T ave, math::adder<T> &xadd)
            {
                return of( seq.begin(), seq.size(), ave, xadd);
            }


        };


#if 0
        //______________________________________________________________________
        //
        //! sample mean absolute deviation
        //______________________________________________________________________
        template <typename T>
        struct mean_absolute_deviation
        {
            //! with copy of original data or previous call to median
            static inline T of(writable<T> &temp, const T med, math::adder<T> &xadd)
            {
                const size_t n = temp.size();
                switch(n)
                {
                    case 0: return 0;
                    case 1: return 0;
                    default:
                        break;
                }

                for(size_t i=n;i>0;--i)
                {
                    temp[i] = fabs(temp[i]-med);
                }

                return 0; //sorted::sum(temp,sorted::by_value)/n;
            }

        };
#endif

    };


}

#endif

