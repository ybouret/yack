//! \file

#ifndef YACK_MATH_ORTHO_FAMILY_INCLUDED
#define YACK_MATH_ORTHO_FAMILY_INCLUDED 1

#include "yack/apex/alga.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace math
    {

        //! computing orthogonal family for a set of independent vector
        struct ortho_family
        {

            //! fundamental algorithm
            /**
             Q.rows = Q.cols = P.cols
             */
            static bool make(matrix<apq> &Q, const matrix<apq> &P);

            //! simplify/univocal
            template <typename T> static inline
            bool make(matrix<apz> &Q, const matrix<T> &P, const bool univocal)
            {
                matrix<apq> QQ(Q.rows,Q.cols);
                {
                    matrix<apq> PP(P,transmogrify);
                    if(!make(QQ,PP)) return false;
                }
                if(univocal) alga::univocal_rows(QQ); else alga::simplify_rows(QQ);
                Q.apply(numerator_of,QQ);
                return true;
            }


            //! simplify/unicoval
            template <typename T, typename U> static inline
            bool build(matrix<T> &Q, const matrix<U> &P, const bool univocal)
            {
                matrix<apq> QQ(Q.rows,Q.cols);
                {
                    matrix<apq> PP(P,transmogrify);
                    if(!make(QQ,PP)) return false;
                }
                if(univocal) alga::univocal_rows(QQ); else alga::simplify_rows(QQ);
                Q.apply(numerator_to<T>,QQ);
                return true;
            }



            //! get numerator of simplified rational
            static apz numerator_of(const apq &);

            //! function for matrix apply
            template <typename T> static inline T numerator_to(const apq &q)
            {
                assert(1==q.den);
                return q.num.cast_to<T>();
            }

        };
        
    }
}

#endif

