
//! \file

#ifndef YACK_MATH_TAO_V1_INCLUDED
#define YACK_MATH_TAO_V1_INCLUDED 1

#include "yack/setup.hpp"

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


                
            };

        }

    }

}

#endif

