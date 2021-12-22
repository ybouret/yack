//! \file

#ifndef YACK_MATH_LOOK_FOR_INCLUDED
#define YACK_MATH_LOOK_FOR_INCLUDED 1

#include "yack/math/numeric.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! look for values on ranges/sequences
        //
        //______________________________________________________________________
        template <typename T>
        struct look_for
        {
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type); //!< aliases

            //__________________________________________________________________
            //
            //! nullity for a range of given values
            /**
             - find max(|values|)
             - if(ftol<=0) then ftol = card(values) * epsilon * vmax
             - set to zero and return number of variables below threshold
             */
            //__________________________________________________________________
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
                if(ftol<=0) ftol      = n * numeric<T>::epsilon;
                const_type  threshold = a*ftol;

                // second pass: cut values below threshold
                size_t ker = 0;
                for(ITERATOR it=curr;it!=last;++it)
                {
                    mutable_type &x = *it;
                    if(fabs(x)<=threshold) {
                        x=0;
                        ++ker;
                    }
                }
                return ker;
            }

            //__________________________________________________________________
            //
            //! nullity for a sequence
            //__________________________________________________________________
            template <typename SEQUENCE> static inline
            size_t nullity(SEQUENCE &seq, const T ftol) throw()
            {
                return nullity(seq.begin(),seq.end(),ftol);
            }
            


        };

    }

}

#endif
