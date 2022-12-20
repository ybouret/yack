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
            bool build(matrix<apq> &Q, const matrix<T> &J)
            {
                const  matrix<apq> P(J,transmogrify);
                return make(Q,P);
            }

            template <typename TARGET, typename SOURCE> static inline
            bool construct(matrix<TARGET> &Q, const matrix<SOURCE> &P)
            {
                const matrix<apq> _P(P,transmogrify);
                matrix<apq>       _Q(Q.rows,Q.cols);
                if(!make(_Q,_P)) return false;
                apk::simplify_rows(_Q);
                //std::cerr << "_Q=" << _Q << std::endl;
                Q.apply(numerator_to<TARGET>,_Q);
                return true;
            }

            template <typename T> static inline T numerator_to(const apq &q)
            {
                assert(1==q.den);
                return q.num.cast_to<T>();
            }


        };
        
    }
}

#endif

