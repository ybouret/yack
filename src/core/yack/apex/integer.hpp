
//! \file

#ifndef YACK_APEX_INTEGER_INCLUDED
#define YACK_APEX_INTEGER_INCLUDED 1

#include "yack/apex/natural.hpp"

namespace yack
{
    
    namespace apex
    {
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
            const sign_type s;
            const natural   n;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~integer() throw();                //!< cleanup
            integer();                                 //!< default=0
            integer(const int_type z);                 //!< from integral type
            integer(const integer &z);                 //!< copy
            integer(const sign_type, const natural &); //!< s+n
            integer & operator=(const integer  &);     //!< assign by copy/xch
            integer & operator=(const int_type z);     //!< assign by copy/xch

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
            void xch(integer &) throw();
            

            //! display
            friend std::ostream &operator<<(std::ostream &, const integer &);
            
            //__________________________________________________________________
            //
            // comparison
            //__________________________________________________________________
#define     YACK_APZ_REP_NO_THROW(PROLOG,CALL)                           \
/**/        inline PROLOG (const integer &lhs, const integer &rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }                  \
/**/        inline PROLOG (const integer &lhs, const int_type rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }                  \
/**/        inline PROLOG (const int_type lhs, const integer &rhs) throw() \
/**/        { const handle L(lhs), R(rhs); return CALL; }                  \

            YACK_APZ_REP_NO_THROW(static int compare,cmp(L,R))

#define     YACK_APZ_REP_CMP(OP) YACK_APZ_REP_NO_THROW(friend bool operator OP, cmp(L,R) OP 0)

            YACK_APZ_REP_CMP(==)
            YACK_APZ_REP_CMP(!=)
            YACK_APZ_REP_CMP(<=)
            YACK_APZ_REP_CMP(>=)
            YACK_APZ_REP_CMP(<)
            YACK_APZ_REP_CMP(>)

#define     YACK_APZ_REP(OP,FCN) \
/**/ inline friend integer operator OP (const integer &lhs, const integer &rhs)\
/**/ { const handle L(lhs), R(rhs); return FCN(L,R); }                         \
/**/ inline friend integer operator OP (const integer &lhs, const int_type rhs)\
/**/ { const handle L(lhs), R(rhs); return FCN(L,R); }                         \
/**/ inline friend integer operator OP (const int_type lhs, const integer &rhs)\
/**/ { const handle L(lhs), R(rhs); return FCN(L,R); } \
/**/ inline integer & operator OP##=(const integer &rhs) \
/**/ { const handle L(*this), R(rhs); integer res = FCN(L,R); xch(res); return *this; }\
/**/ inline integer & operator OP##=(const int_type rhs) \
/**/ { const handle L(*this), R(rhs); integer res = FCN(L,R); xch(res); return *this; }

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
            integer  operator-() const;
            integer &operator--();      //!< pre  decrease operator
            integer  operator--(int);   //!< post decrease operator
            YACK_APZ_REP(-,sub)

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            YACK_APZ_REP(*,mul)



        private:
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

            class handle : public handle_, public natural::handle
            {
            public:
                explicit handle(const integer &) throw();
                explicit handle(const int_type ) throw();
                virtual ~handle() throw();

                const sign_type s;
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(handle);
            };

            static int       cmp(const  handle &lh, const handle &rh) throw();
            static sign_type scmp(const handle &lh, const handle &rh) throw();
            static integer   add(const handle &lh, const handle &rh);
            static integer   sub(const handle &lh, const handle &rh);
            static integer   mul(const handle &lh, const handle &rh);

        };
    }
    
    typedef apex::integer apz;
    
}

#endif
        
