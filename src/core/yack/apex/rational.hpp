//! \file

#ifndef YACK_APEX_RATIONAL_INCLUDED
#define YACK_APEX_RATIONAL_INCLUDED 1

#include "yack/apex/integer.hpp"

namespace yack
{

    namespace apex
    {
        //______________________________________________________________________
        //
        //
        //! rational number
        //
        //______________________________________________________________________
        class rational : public number
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char clid[]; //!< "apq"

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const integer num; //!< numerator
            const natural den; //!< denominator>0

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~rational() noexcept;                   //!< cleanup
            rational();                                     //!< 0/1
            rational(const rational &);                     //!< copy
            rational(const int_type);                       //!< num/1
            rational(const integer &);                      //!< num/1
            rational(const sign_type, const natural &);     //!< num/1 with sign
            rational(const natural &);                      //!< num/1
            rational&operator=(const rational &);           //!< assign by copy/xch
            rational(randomized::bits &,const size_t bits); //!< random
            rational & operator=(const int_type);           //!< num/1
            

            //! inline binary constructors
#define     YACK_APQ_CTOR(NTYPE,DTYPE) \
/**/        inline rational(const NTYPE N, const DTYPE D) :\
/**/        number(), num(N), den(D) { setup(); }

            YACK_APQ_CTOR(int_type,uint_type)
            YACK_APQ_CTOR(integer&,natural&)
            YACK_APQ_CTOR(int_type,natural&)
            YACK_APQ_CTOR(integer&,uint_type)
            YACK_APQ_CTOR(natural&,natural&)

            rational(randomized::bits &, const size_t nbits, const size_t dbits); //!< random


            //__________________________________________________________________
            //
            // compare functions
            //__________________________________________________________________
            //! preparing all protopype
#define     YACK_APQ_REP(PROLOG,FUNCTION,EPILOG)                              \
/**/        PROLOG FUNCTION (const rational &lhs, const rational &rhs) EPILOG \
/**/        PROLOG FUNCTION (const rational &lhs, const integer  &rhs) EPILOG \
/**/        PROLOG FUNCTION (const integer  &lhs, const rational &rhs) EPILOG \
/**/        PROLOG FUNCTION (const rational &lhs, const int_type  rhs) EPILOG \
/**/        PROLOG FUNCTION (const int_type  lhs, const rational &rhs) EPILOG \
/**/        PROLOG FUNCTION (const rational &lhs, const natural  &rhs) EPILOG \
/**/        PROLOG FUNCTION (const natural  &lhs, const rational &rhs) EPILOG

            //! declare all comparison function
            YACK_APQ_REP(static int,compare,;)




            //__________________________________________________________________
            //
            // comparison operators
            //__________________________________________________________________

            //! preparing all prototypes
#define     YACK_APQ_REP_CMP(OP) YACK_APQ_REP(inline friend bool operator,OP,{ return compare(lhs,rhs) OP 0; })

            //! collect all operators
#define     YACK_APQ_CMP_IMPL()  \
/**/        YACK_APQ_REP_CMP(==) \
/**/        YACK_APQ_REP_CMP(!=) \
/**/        YACK_APQ_REP_CMP(<=) \
/**/        YACK_APQ_REP_CMP(>=) \
/**/        YACK_APQ_REP_CMP(>)  \
/**/        YACK_APQ_REP_CMP(<)


            //! implement all comparisons
            YACK_APQ_CMP_IMPL()

            //__________________________________________________________________
            //
            // operators
            //__________________________________________________________________

            //! declare all operators
#define     YACK_APQ_DECL(OP,CALL) \
/**/        YACK_APQ_REP(inline friend rational operator,OP,{return CALL(lhs,rhs);})\
/**/        inline rational & operator OP##=(const rational &rhs) { rational _ = CALL(*this,rhs); xch(_); return *this; } \
/**/        inline rational & operator OP##=(const integer  &rhs) { rational _ = CALL(*this,rhs); xch(_); return *this; } \
/**/        inline rational & operator OP##=(const natural  &rhs) { rational _ = CALL(*this,rhs); xch(_); return *this; } \
/**/        inline rational & operator OP##=(const int_type  rhs) { rational _ = CALL(*this,rhs); xch(_); return *this; }


            //__________________________________________________________________
            //
            // ADD
            //__________________________________________________________________
            YACK_APQ_REP(static rational,add,;)
            YACK_APQ_DECL(+,add)
            rational  operator+() const; //!< self
            rational &operator++();      //!< pre  increase operator
            rational  operator++(int);   //!< post increase operator

            //__________________________________________________________________
            //
            // sub
            //__________________________________________________________________
            YACK_APQ_REP(static rational,sub,;)
            YACK_APQ_DECL(-,sub)
            rational  operator-() const; //!< -self
            rational &operator--();      //!< pre  increase operator
            rational  operator--(int);   //!< post increase operator
            void      neg() noexcept;     //!< in-place no-throw change sign


            //__________________________________________________________________
            //
            // mul
            //__________________________________________________________________
            YACK_APQ_REP(static rational,mul,;)
            YACK_APQ_DECL(*,mul)

            //__________________________________________________________________
            //
            // div
            //__________________________________________________________________
            YACK_APQ_REP(static rational,div,;)
            YACK_APQ_DECL(/,div)

            //__________________________________________________________________
            //
            // other operators
            //__________________________________________________________________
            rational abs()  const; //!< |q|
            rational mod2() const; //!< |q|^2
            void     ldz();        //!< q=0/1
            
            //__________________________________________________________________
            //
            // serializable interface
            //__________________________________________________________________
            virtual const char * class_uid()       const noexcept; //!< clid
            virtual size_t       serialize(ios::ostream &) const; //!< num+den
            static  rational     construct(ios::istream &, size_t &cumul); //!< from num+den

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! no-throw exchange
            void xch(rational &) noexcept;


            //! display
            friend std::ostream &operator<<(std::ostream &, const rational &);

            //! convert to floating point
            template <typename T> inline T to() const
            {
                switch(num.s)
                {
                    case negative: return - natural::ratio<T>(num.n,den);
                    case __zero__: return 0;
                    case positive: return   natural::ratio<T>(num.n,den);
                }
                // never get here
                return 0;
            }

        private:
            void reduce();
            void zcheck() const;
            void setup(); //!< zcheck()/reduce()
        };

    }
    
    typedef apex::rational apq; //!< alias

}

#endif

