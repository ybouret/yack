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
                matrix<apq> _P(P,transmogrify);
                matrix<apq> _Q(Q.rows,Q.cols);
                if(!make(_Q,_P)) return false;
                if(univocal) alga::univocal_rows(_Q); else alga::simplify_rows(_Q);
                Q.apply(numerator_of,_Q);
                return true;
            }


            //! simplify/unicoval
            template <typename T, typename U> static inline
            bool build(matrix<T> &Q, const matrix<U> &P, const bool univocal)
            {
                matrix<apq> _P(P,transmogrify);
                matrix<apq> _Q(Q.rows,Q.cols);
                if(!make(_Q,_P)) return false;
                if(univocal) alga::univocal_rows(_Q); else alga::simplify_rows(_Q);
                Q.apply(numerator_to<T>,_Q);
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

