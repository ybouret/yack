//! \file

#ifndef YACK_DATA_STATS_INCLUDED
#define YACK_DATA_STATS_INCLUDED 1

#include "yack/sequence/vector.hpp"
#include "yack/sort/sum.hpp"
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

            //! from iterator with sorted sum
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

            //! for a whole sequence
            template <typename SEQUENCE> static
            inline T of(SEQUENCE &seq)
            {
                return of_( seq.begin(), seq.size() );
            }

            //! for a while sequence, with sorted sum
            template <typename SEQUENCE, typename SEQTEMP> static
            inline T of(SEQUENCE &seq, SEQTEMP &temp)
            {
                return of_( seq.begin(), seq.size(), temp );
            }


        };


        //______________________________________________________________________
        //
        //! sample variance
        //______________________________________________________________________
        template <typename T>
        struct variance
        {
            //! from iterator
            template <typename ITERATOR> static inline
            T of_(ITERATOR it, const size_t n, const T ave)
            {
                assert(n>1);
                T sum = 0, sum2 = 0;
                for(size_t i=n;i>0;--i)
                {
                    const T del = *(it++) - ave;
                    sum  += del;
                    sum2 += del*del;
                }

                return  ((sum2 - (sum*sum)/n)/(n-1));
            }

            //! from iterator with sorted sum
            template <typename ITERATOR, typename SEQUENCE> static inline
            T of_(ITERATOR it, const size_t n, const T ave, SEQUENCE &temp)
            {
                assert(n>1);
                temp.free();
                for(size_t i=n;i>0;--i)
                {
                    const T  del = *(it++) - ave;
                    const T  sqr = del*del;
                    temp.push_back(sqr);
                }
                return  ( sorted::sum(temp,sorted::by_value) / (n-1) );
            }


            //! for a whole sequence
            template <typename SEQUENCE> static
            inline T of(SEQUENCE &seq, const T ave)
            {
                return of_( seq.begin(), seq.size(), ave );
            }

            //! for a whole sequence with sorted sum
            template <typename SEQUENCE, typename SEQTEMP> static
            inline T of(SEQUENCE &seq, const T ave, SEQTEMP &temp)
            {
                return of_( seq.begin(), seq.size(), ave, temp );
            }

        };
        
        
    }
}

#endif

