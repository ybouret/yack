//! \file

#ifndef YACK_MATH_LOOK_FOR_INCLUDED
#define YACK_MATH_LOOK_FOR_INCLUDED 1

#include "yack/math/numeric.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    namespace math
    {

        template <typename T>
        struct look_for
        {
            YACK_DECL_ARGS(T,type);

            template <typename ITERATOR> static inline
            size_t nullity(const ITERATOR curr,
                           const ITERATOR last,
                           T              ftol) throw()
            {
                // first pass: count and find max value
                mutable_type a = 0;
                size_t       n = 0;
                for(ITERATOR it=curr;it!=last;++it,++n)
                {
                    const_type x = *it;
                    const_type t = fabs(x);
                    if(t>a)    a = t;
                }

                // adjust ftol
                if(ftol<=0) ftol = n * numeric<T>::epsilon * a;

                // second pass: cut value
                size_t ker = 0;
                for(ITERATOR it=curr;it!=last;++it)
                {
                    mutable_type &x = *it;
                    if(fabs(x)<=ftol) {
                        x=0;
                        ++ker;
                    }
                }
                return ker;
            }

            template <typename SEQUENCE> static inline
            size_t nullity(SEQUENCE &seq, const T ftol) throw()
            {
                return nullity(seq.begin(),seq.end(),ftol);
            }
            


        };

    }

}

#endif
