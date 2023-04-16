//! \file

#ifndef YACK_APEX_XREAL_INCLUDED
#define YACK_APEX_XREAL_INCLUDED 1

#include "yack/signs.hpp"
#include <iostream>

namespace yack
{
    namespace apex
    {

        //______________________________________________________________________
        //
        //
        //! value = m * 2^p
        //
        //______________________________________________________________________
        template <typename T>
        class xreal
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //___________________________________________________________________
            static const T   maximum; //!< [FLT|DBL|LDBL]_MAX
            static const T   minimum; //!< [FLT|DBL|LDBL]_MIN
            static const int max_exp; //!< [FLT|DBL|LDBL]_MAX_EXP
            static const int min_exp; //!< [FLT|DBL|LDBL]_MIN_EXP
            static const T   epsilon; //!< [FLT|DBL|LDBL]_EPSILON
            
            //__________________________________________________________________
            //
            // C++
            //___________________________________________________________________
            xreal(const T     r);                      //!< setup with frexp
            xreal(const xreal &) noexcept;             //!< noexcept copy
            ~xreal() noexcept;                         //!< cleanup
            xreal & operator=(const xreal &) noexcept; //!< assign

            //__________________________________________________________________
            //
            // methods
            //___________________________________________________________________

            static xreal _1() noexcept;

            //! output
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
            // converson
            //__________________________________________________________________
            T operator*() const;

            //__________________________________________________________________
            //
            // unary operators
            //__________________________________________________________________
            xreal operator+() const noexcept; //!< unary+
            xreal operator-() const noexcept; //!< unary-

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            xreal &             operator*=(const xreal rhs); //!< two stages multiplication
            inline friend xreal operator*(xreal lhs, const xreal rhs) {
                return    lhs *= rhs;
            } //!< return lhs *  rhs

            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            xreal &      operator/=(const xreal rhs); //!< two stages division
            inline friend xreal operator/(xreal lhs, const xreal rhs) {
                return    lhs /= rhs;
            } //!< return lhs /  rhs


            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            xreal &      operator+=(const xreal rhs); //!< addition
            inline friend xreal operator+(xreal lhs, const xreal rhs) {
                return    lhs += rhs;
            } //!< return lhs +  rhs

            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            xreal &      operator-=(const xreal rhs); //!< subtraction
            inline friend xreal operator-(xreal lhs, const xreal rhs) {
                return    lhs -= rhs;
            } //!< return lhs -  rhs

            xreal & power(unsigned n) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int        p; //!< exponent
            const T          m; //!< mantissa

        private:
            xreal(const int p_, const T m_) noexcept;
            bool is_valid() const noexcept;
        };
        
    }
}

#endif

