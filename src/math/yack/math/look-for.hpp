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
            //! nullity finding
            //__________________________________________________________________
            struct nullity
            {
                //______________________________________________________________
                //
                //! nullity for a range of given values
                /**
                 - find max(|values|)
                 - if(ftol<=0) then ftol = card(values) * epsilon * vmax
                 - set to zero and return number of variables below threshold
                 */
                //______________________________________________________________
                template <typename ITERATOR> static inline
                size_t of(const ITERATOR curr,
                          const ITERATOR last,
                          T              ftol) throw()
                {
                    // first pass: count and find max value
                    mutable_type a = 0;
                    size_t       n = 0;
                    for(ITERATOR it=curr;it!=last;++it,++n)
                    {
                        const_type x = *it;
                        const_type t = std::abs(x);
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
                        if(std::abs(x)<=threshold) {
                            x=0;
                            ++ker;
                        }
                    }
                    return ker;
                }

                //______________________________________________________________
                //
                //! nullity for a sequence
                //______________________________________________________________
                template <typename SEQUENCE> static inline
                size_t of(SEQUENCE &seq, const T ftol) throw()
                {
                    return of(seq.begin(),seq.end(),ftol);
                }
            };

            //__________________________________________________________________
            //
            //! maximum finding
            //__________________________________________________________________
            struct maximum
            {
                //______________________________________________________________
                //
                //! maximum of a range
                //______________________________________________________________
                template <typename ITERATOR> static inline
                mutable_type of(ITERATOR       curr,
                                const ITERATOR last) throw()
                {
                    mutable_type res = 0;
                    while(curr!=last)
                        res = max_of<mutable_type>(res,*(curr++));
                    return res;
                }

                //______________________________________________________________
                //
                //! maximum of a sequence
                //______________________________________________________________
                template <typename SEQUENCE> static inline
                mutable_type of(SEQUENCE &seq) throw() {
                    return of(seq.begin(),seq.end());
                }

                //______________________________________________________________
                //
                //! maximum of a function of a range
                //______________________________________________________________
                template <typename ITERATOR, typename FUNC> static inline
                mutable_type of(ITERATOR       curr,
                                const ITERATOR last,
                                FUNC          &func) throw()
                {
                    mutable_type res = 0;
                    res = func(res);
                    while(curr!=last)
                        res = max_of<mutable_type>(res,func(*(curr++)));
                    return res;
                }

                //______________________________________________________________
                //
                //! maximum of a function of a sequence
                //______________________________________________________________
                template <typename SEQUENCE, typename FUNC> static inline
                mutable_type of(SEQUENCE &seq, FUNC &func) throw() {
                    return of(seq.begin(),seq.end(),func);
                }


            };


        };

    }

}

#endif
