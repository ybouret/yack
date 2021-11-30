
//! \file

#ifndef YACK_MATH_TAO_V1_INCLUDED
#define YACK_MATH_TAO_V1_INCLUDED 1

#include "yack/math/api.hpp"

namespace yack
{
    namespace math
    {

        namespace tao
        {

            //__________________________________________________________________
            //
            //
            //! array/array operation
            //
            //__________________________________________________________________
            struct v1
            {
                //! a = b+c
                template <typename A, typename B, typename C> static inline
                void add( A &a, B &b, C &c)
                {
                    assert(a.size()<=b.size());
                    assert(a.size()<=c.size());
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] = b[i] + c[i];
                    }
                }

                //! a = b-c
                template <typename A, typename B, typename C> static inline
                void sub( A &a, B &b, C &c)
                {
                    assert(a.size()<=b.size());
                    assert(a.size()<=c.size());
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] = b[i] - c[i];
                    }
                }

                //! a += b
                template <typename A, typename B> static inline
                void add( A &a, B &b )
                {
                    assert(a.size()<=b.size());
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] += b[i];
                    }
                }


                //! a -= b
                template <typename A, typename B> static inline
                void sub( A &a, B &b )
                {
                    assert(a.size()<=b.size());
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] -= b[i];
                    }
                }

                //! a = x*b
                template <typename A, typename X, typename B> static inline
                void mul( A &a, X &x, B &b)
                {
                    assert(a.size()<=b.size());
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] = x * b[i];
                    }
                }

                //! a *= x
                template <typename A, typename X> static inline
                void mul( A &a, X &x )
                {
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] *= x;
                    }
                }

                //! a = b + x*c
                template <typename A, typename B, typename X, typename C> static inline
                void muladd(A &a, B &b, X &x, C &c)
                {
                    assert(a.size()<=b.size());
                    assert(a.size()<=c.size());
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] = b[i] + x * c[i];
                    }
                }

                //! a = b - x*c
                template <typename A, typename B, typename X, typename C> static inline
                void mulsub(A &a, B &b, X &x, C &c)
                {
                    assert(a.size()<=b.size());
                    assert(a.size()<=c.size());
                    for(size_t i=a.size();i>0;--i)
                    {
                        a[i] = b[i] - x * c[i];
                    }
                }

                template <typename T>
                struct mod2
                {
                    template <typename A, typename B> static inline
                    T of(A &a, B &b)
                    {
                        assert(a.size()<=b.size());
                        T sum = 0;
                        for(size_t i=a.size();i>0;--i)
                        {
                            typename A::mutable_type d = a[i] - b[i];
                            sum += mod2_of<typename A::mutable_type>(d);
                        }
                        return sum;
                    }

                };

                
            };

        }

    }

}

#endif

