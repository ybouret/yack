//! \file

#ifndef YACK_MATH_POLYNOMIAL_INCLUDED
#define YACK_MATH_POLYNOMIAL_INCLUDED 1

#include "yack/sequence/arrays.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! polynomial utilities
        //
        //______________________________________________________________________
        struct polynomial
        {

            //__________________________________________________________________
            //
            //
            //! Lagrange's interpolation
            //
            //______________________________________________________________________
            template <typename T>
            class interpolate : public arrays_of<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef arrays_of<T>                  tableaux;   //!< alias
                typedef typename tableaux::array_type array_type; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit interpolate(const size_t ndat=0); //!< setup with initial data
                virtual ~interpolate() noexcept;            //!< cleanup

                //______________________________________________________________
                //
                //! generic call
                //______________________________________________________________
                template <typename X,typename Y> inline
                T operator()(T x, X &xa, Y &ya, T &dy)
                {
                    assert( xa.size() == ya.size() );
                    const size_t n   = xa.size(); this->ensure(n);
                    size_t       ns  = 1;
                    {
                        T del = std::abs(x-xa[1]);
                        c[1]  = d[1] = ya[1];
                        for(size_t i=n;i>1;--i)
                        {
                            c[i] = d[i] = ya[i];
                            const T tmp = std::abs(x-xa[i]);
                            if(tmp<del)
                            {
                                del = tmp;
                                ns  = i;
                            }
                        }
                    }
                    T y = ya[ns--];
                    dy  = 0;
                    for(size_t m=1;m<n;m++)
                    {
                        const size_t top = n-m;
                        for(size_t i=1;i<=top;++i)
                        {
                            const T ho=xa[i]-x;
                            const T hp=xa[i+m]-x;
                            const T w=c[i+1]-d[i];
                            T       den=ho-hp;
                            if( std::abs(den) <= 0 ) throw_singular();
                            den=w/den;
                            d[i]=hp*den;
                            c[i]=ho*den;
                        }
                        y += (dy=(2*ns < top ? c[ns+1] : d[ns--]));
                    }
                    return y;
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(interpolate);
                array_type &c;
                array_type &d;
                void throw_singular() const;
            };


        };

    }

}

#endif

