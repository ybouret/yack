//! \file

#ifndef YACK_MATH_ORTHO_FAMILY_INCLUDED
#define YACK_MATH_ORTHO_FAMILY_INCLUDED 1

#include "yack/apex/kernel.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace math
    {

        //! computing orthogonal family for a set of independent vector
        struct ortho_family
        {

            //! fundamental algorithm: Q.rows = Q.cols = P.cols
            static bool make(matrix<apq> &Q, const matrix<apq> &P);


            //! copy integral type to apq, return make
            template <typename T> static inline
            bool build(matrix<apq> &Q, const matrix<T> &P)
            {
                const  matrix<apq> _P(P,transmogrify);
                return make(Q,_P);
            }

            //! copy integral type to apq, return simplified apz
            template <typename T> static inline
            bool build(matrix<apz> &Q, const matrix<T> &P)
            {
                const matrix<apq> _P(P,transmogrify);
                matrix<apq>       _Q(Q.rows,Q.cols);
                if(!make(_Q,_P)) return false;
                apk::simplify_rows(_Q);
                Q.apply(numerator_of,_Q);
                return true;
            }

            //! copy P to apq, compute simplified Q
            template <typename TARGET, typename SOURCE> static inline
            bool construct(matrix<TARGET> &Q, const matrix<SOURCE> &P)
            {
                const matrix<apq> _P(P,transmogrify);
                matrix<apq>       _Q(Q.rows,Q.cols);
                if(!make(_Q,_P)) return false;
                apk::simplify_rows(_Q);
                Q.apply(numerator_to<TARGET>,_Q);
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

