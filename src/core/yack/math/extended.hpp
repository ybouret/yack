
//! \file

#ifndef YACK_MATH_EXETENDED_INCLUDED
#define YACK_MATH_EXETENDED_INCLUDED 1

#include "yack/signs.hpp"
#include <iostream>

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! value = m * 2^p
    //
    //__________________________________________________________________________
    template <typename T>
    class extended
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        static const T        maximum;  //!< [FLT|DBL|LDBL]_MAX
        static const T        minimum;  //!< [FLT|DBL|LDBL]_MIN
        static const int      max_exp;  //!< [FLT|DBL|LDBL]_MAX_EXP
        static const int      min_exp;  //!< [FLT|DBL|LDBL]_MIN_EXP
        static const T        epsilon;  //!< [FLT|DBL|LDBL]_EPSILON
        static const unsigned mant_dig; //!< [FLT|DBL|LDBL]_MANT_DIG

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        extended() noexcept;                             //!< setup 0
        extended(const T     r);                         //!< setup with frexp
        extended(const extended &) noexcept;             //!< noexcept copy
        virtual ~extended() noexcept;                    //!< cleanup
        extended & operator=(const extended &) noexcept; //!< assign

        //__________________________________________________________________
        //
        // methods
        //___________________________________________________________________

        static extended _1() noexcept; //!< 0.5 * 2^1

        //! output
        friend inline std::ostream & operator<<(std::ostream &os,const extended<T> &x)
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
        // conversion
        //__________________________________________________________________
        T operator*() const; //!< ldexp

        //__________________________________________________________________
        //
        // unary operators
        //__________________________________________________________________
        extended operator+() const noexcept; //!< unary+
        extended operator-() const noexcept; //!< unary-

        //__________________________________________________________________
        //
        // multiplication
        //__________________________________________________________________
        extended &             operator*=(const extended rhs); //!< two stages multiplication
        inline friend extended operator*(extended lhs, const extended rhs) {
            return    lhs *= rhs;
        } //!< return lhs *  rhs

        //__________________________________________________________________
        //
        // division
        //__________________________________________________________________
        extended &      operator/=(const extended rhs); //!< two stages division
        inline friend extended operator/(extended lhs, const extended rhs) {
            return    lhs /= rhs;
        } //!< return lhs /  rhs


        //__________________________________________________________________
        //
        // addition
        //__________________________________________________________________
        extended &      operator+=(const extended rhs); //!< addition
        inline friend extended operator+(extended lhs, const extended rhs) {
            return    lhs += rhs;
        } //!< return lhs +  rhs

        //__________________________________________________________________
        //
        // subtraction
        //__________________________________________________________________
        extended &      operator-=(const extended rhs); //!< subtraction
        inline friend extended operator-(extended lhs, const extended rhs) {
            return    lhs -= rhs;
        } //!< return lhs -  rhs


        //__________________________________________________________________
        //
        // signs
        //__________________________________________________________________

        //! sign_type from comparison
        static sign_type scompare(const extended &lhs,
                                  const extended &rhs);

        //! absolute value
        extended abs() const noexcept;

        //! sqrt
        extended sqrt() const;

        //! 10^exp10
        T exp10() const;

        //! this ^ n
        extended upower(unsigned n) const;

        

        //__________________________________________________________________
        //
        // comparisons
        //__________________________________________________________________
        inline friend bool operator==(const extended lhs, const extended rhs) { return __zero__ == scompare(lhs,rhs); } //!< test
        inline friend bool operator!=(const extended lhs, const extended rhs) { return __zero__ != scompare(lhs,rhs); } //!< test
        inline friend bool operator< (const extended lhs, const extended rhs) { return negative == scompare(lhs,rhs); } //!< test
        inline friend bool operator> (const extended lhs, const extended rhs) { return positive == scompare(lhs,rhs); } //!< test
        inline friend bool operator<=(const extended lhs, const extended rhs)
        {
            switch(scompare(lhs,rhs))
            {
                case negative:
                case __zero__: return true;
                case positive: return false;
            }
        } //!< test

        inline friend bool operator>=(const extended lhs, const extended rhs)
        {
            switch(scompare(lhs,rhs))
            {
                case positive:
                case __zero__: return true;
                case negative: return false;
            }
        } //!< test
        
        //__________________________________________________________________
        //
        // members
        //__________________________________________________________________
        const int        p; //!< exponent
        const T          m; //!< mantissa

    private:
        extended(const int p_, const T m_) noexcept;
        bool is_valid() const noexcept;
    };

#if !defined(_MSC_VER)
    //! declaring constants is necessary
#define YACK_EXTENDED(VALUE)                                \
template <> const float       extended<float>      ::VALUE; \
template <> const double      extended<double>     ::VALUE; \
template <> const long double extended<long double>::VALUE

    YACK_EXTENDED(epsilon);     //!< epsilon
    YACK_EXTENDED(minimum);     //!< minimum
    YACK_EXTENDED(maximum);     //!< maximum

#endif

}

#endif

