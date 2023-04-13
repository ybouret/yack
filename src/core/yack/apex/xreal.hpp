//! \file

#ifndef YACK_APEX_XREAL_INCLUDED
#define YACK_APEX_XREAL_INCLUDED 1

#include "yack/signs.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        //! value = m * 10^p
        template <typename T>
        class xreal
        {
        public:
            static const T   maximum;
            static const T   minimum;
            
            const int        p;
            const T          m;

            xreal(const T     r);
            xreal(const xreal &) noexcept;
            ~xreal() noexcept;
            xreal & operator=(const xreal &) noexcept;

            friend inline std::ostream & operator<<(std::ostream &os,const xreal<T> &x)
            {
                os << '(' << x.m;
                switch(x.p)
                {
                    case -1: os << "/2"; goto END;
                    case  0:             goto END;
                    case  1: os << "*2"; goto END;
                    default:
                        break;
                }
                if(x.p<0)
                {
                    os << "*2^(" << x.p << ")";
                }
                else
                {
                    os << "*2^" << x.p;
                }
            END:
                os << ')';
                return os;
            }

            //__________________________________________________________________
            //
            // unary operators
            //__________________________________________________________________
            xreal operator+() const noexcept;
            xreal operator-() const noexcept;

            xreal &      operator*=(const xreal rhs);
            inline friend xreal operator*(xreal lhs, const xreal rhs)
            {
                return lhs *= rhs;
            }

            xreal &      operator/=(const xreal rhs);
            inline friend xreal operator/(xreal lhs, const xreal rhs)
            {
                return lhs /= rhs;
            }


        private:
            xreal(const int p_, const T m_) noexcept;
        };
        
    }
}

#endif

