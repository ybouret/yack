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
            virtual ~rational() throw();                //!< cleanup
            rational();                                 //!< 0/1
            rational(const rational &);                 //!< copy
            rational(const int_type);                   //!< num/1
            rational(const integer &);                  //!< num/1
            rational(const sign_type, const natural &); //!< num/1
            rational(const natural &);                  //!< num/1
            rational&operator=(const rational &);       //!< assign by copy/xch

            //! inline binary constructors
#define     YACK_APQ_CTOR(NTYPE,DTYPE) \
/**/        inline rational(const NTYPE N, const DTYPE D) :\
/**/        number(), num(N), den(D) { setup(); }

            YACK_APQ_CTOR(int_type,uint_type)
            YACK_APQ_CTOR(integer&,natural&)
            YACK_APQ_CTOR(int_type,natural&)
            YACK_APQ_CTOR(integer&,uint_type)
            YACK_APQ_CTOR(natural&,natural&)


            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________
            static int compare(const rational &lhs, const rational &rhs); //!< comparison
            static int compare(const rational &lhs, const integer  &rhs); //!< comparison
            static int compare(const integer  &lhs, const rational &rhs); //!< comparison
            static int compare(const rational &lhs, const int_type  rhs); //!< comparison
            static int compare(const int_type  lhs, const rational &rhs); //!< comparison
            static int compare(const rational &lhs, const natural  &rhs); //!< comparison
            static int compare(const natural  &lhs, const rational &rhs); //!< comparison


            //__________________________________________________________________
            //
            // serializable interface
            //__________________________________________________________________
            virtual const char * class_uid()       const throw();
            virtual size_t       serialize(ios::ostream &) const;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! no-throw exchange
            void xch(rational &) throw();


            //! display
            friend std::ostream &operator<<(std::ostream &, const rational &);


        private:
            void reduce();
            void zcheck() const;
            void setup(); //!< zcheck()/reduce()
        };

    }
    
    typedef apex::rational apq; //!< alias

}

#endif

