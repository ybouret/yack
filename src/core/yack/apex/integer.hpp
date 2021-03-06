//! \file

#ifndef YACK_APEX_INTEGER_INCLUDED
#define YACK_APEX_INTEGER_INCLUDED 1

#include "yack/apex/natural.hpp"

namespace yack
{
    
    namespace apex
    {

        class rational;

        //______________________________________________________________________
        //
        //
        //! drop-in signed replacement
        //
        //______________________________________________________________________
        class integer : public number
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char clid[]; //!< "apz"

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const sign_type s; //!< sign
            const natural   n; //!< absolute value

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~integer() throw();                      //!< cleanup
            integer();                                       //!< default=0
            integer(const int_type z);                       //!< from integral type
            integer(const integer &z);                       //!< copy
            integer(const sign_type, const natural &);       //!< s+n
            integer(const natural &);                        //!< (positive) value
            integer & operator=(const integer  &);           //!< assign by copy/xch
            integer & operator=(const int_type z);           //!< assign by copy/xch
            integer(randomized::bits &, const size_t nbits); //!< make exactly nbits, and random/random sign

            //__________________________________________________________________
            //
            // serializable interface
            //__________________________________________________________________
            virtual const char * class_uid()       const throw(); //!< clid
            virtual size_t       serialize(ios::ostream &) const; //!< s+n
            static  integer      construct(ios::istream &, size_t &cumul); //!< from s+n

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! no-throw exchange
            void xch(integer &) throw();
            

            //! display
            friend std::ostream &operator<<(std::ostream &, const integer &);
            
            //__________________________________________________________________
            //
            // comparison
            //__________________________________________________________________

            //! build API
#define     YACK_APZ_REP_NO_THROW(PROLOG,CALL)                           \
/**/        inline PROLOG (const integer &lhs, const integer &rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }                  \
/**/        inline PROLOG (const integer &lhs, const int_type rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }                  \
/**/        inline PROLOG (const int_type lhs, const integer &rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }                  \
/**/        inline PROLOG (const integer &lhs, const natural &rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }                  \
/**/        inline PROLOG (const natural &lhs, const integer &rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }


            YACK_APZ_REP_NO_THROW(static int compare,cmp(L,R))

            //! build comparison operators
#define     YACK_APZ_REP_CMP(OP) YACK_APZ_REP_NO_THROW(friend bool operator OP, cmp(L,R) OP 0)

            YACK_APZ_REP_CMP(==)
            YACK_APZ_REP_CMP(!=)
            YACK_APZ_REP_CMP(<=)
            YACK_APZ_REP_CMP(>=)
            YACK_APZ_REP_CMP(<)
            YACK_APZ_REP_CMP(>)

            //__________________________________________________________________
            //
            //! build algebraic operators
            //__________________________________________________________________

#define     YACK_APZ_REP(OP,FCN) \
/**/        inline friend integer operator OP (const integer &lhs, const integer &rhs)         \
/**/        { const handle L(lhs), R(rhs); return FCN(L,R); }                                  \
/**/        inline friend integer operator OP (const integer &lhs, const int_type rhs)         \
/**/        { const handle L(lhs), R(rhs); return FCN(L,R); }                                  \
/**/        inline friend integer operator OP (const int_type lhs, const integer &rhs)         \
/**/        { const handle L(lhs), R(rhs); return FCN(L,R); }                                  \
/**/        inline friend integer operator OP (const integer &lhs, const natural &rhs)         \
/**/        { const handle L(lhs), R(rhs); return FCN(L,R); }                                  \
/**/        inline friend integer operator OP (const natural &lhs, const integer &rhs)         \
/**/        { const handle L(lhs), R(rhs); return FCN(L,R); }                                  \
/*          unary ops       */                                                                 \
/**/        inline integer & operator OP##=(const integer &rhs)                                \
/**/        { const handle L(*this), R(rhs); integer res = FCN(L,R); xch(res); return *this; } \
/**/        inline integer & operator OP##=(const int_type rhs)                                \
/**/        { const handle L(*this), R(rhs); integer res = FCN(L,R); xch(res); return *this; } \
/**/        inline integer & operator OP##=(const natural &rhs)                                \
/**/        { const handle L(*this), R(rhs); integer res = FCN(L,R); xch(res); return *this; }

            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            integer  operator+() const; //!< self
            integer &operator++();      //!< pre  increase operator
            integer  operator++(int);   //!< post increase operator
            YACK_APZ_REP(+,add)
            
            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            integer  operator-() const; //!< change sign
            integer &operator--();      //!< pre  decrease operator
            integer  operator--(int);   //!< post decrease operator
            YACK_APZ_REP(-,sub)

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            YACK_APZ_REP(*,mul)

            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            YACK_APZ_REP(/,div)

            //__________________________________________________________________
            //
            // other methods
            //__________________________________________________________________
            integer abs()  const; //!< |z|
            integer mod2() const; //!< |z|^2

            //__________________________________________________________________
            //
            // casting
            //__________________________________________________________________
            //! try to cast to a system integer
            template <typename T> inline
            bool try_cast_to(T &res) const throw()
            {
                switch(s)
                {
                    case __zero__: res=0; return true;
                    case positive: return n.try_cast_to(res);
                    case negative:
                        break;
                }
                const int_type imin = int_type( integral_for<T>::minimum );
                const handle   hmin(imin);
                const handle   self(*this);
                if( cmp(self,hmin) < 0 ) return false;
                res = -int_type( n.lsu() );
                return true;
            }


            //! cast with overflow check
            template <typename T> inline
            T cast_to(const char *who=NULL) const
            {
                T res=0;
                if(!try_cast_to(res)) cast_overflow(who);
                return res;
            }

            //! signed natural
            double to_double() const;

            //! simplify natural parts
            static void simplify(integer &num, integer &den);

        private:
            friend class rational;
            
            //! data to hold an uint_type to use natural::handle
            class handle_
            {
            public:
                virtual ~handle_() throw();
            protected:
                explicit handle_() throw();
                explicit handle_(const int_type) throw();
                uint_type u;
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(handle_);
            };

            //! local handle for multiple types of calls
            class handle : public handle_, public natural::handle
            {
            public:
                explicit handle(const integer  &) throw();
                explicit handle(const int_type &) throw();
                explicit handle(const natural  &) throw();
                virtual ~handle() throw();

                const sign_type s;
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(handle);
            };

            static int        cmp(const handle &lh, const handle &rh) throw();
            static sign_type scmp(const handle &lh, const handle &rh) throw();
            static integer    add(const handle &lh, const handle &rh);
            static integer    sub(const handle &lh, const handle &rh);
            static integer    mul(const handle &lh, const handle &rh);
            static integer    div(const handle &lh, const handle &rh);

        };
    }
    
    typedef apex::integer apz; //!< alias
    
}

#endif

