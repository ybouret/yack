//! \file

#ifndef YACK_MATH_MULTIPLIER_INCLUDED
#define YACK_MATH_MULTIPLIER_INCLUDED 1

#include "yack/ordered/list.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {
        namespace core
        {
            //__________________________________________________________________
            //
            //! a real with its integer exponent
            //__________________________________________________________________
            template <typename T>
            class tagged_real
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                inline   tagged_real(const T args) throw() : value(args), exponent(rexp(value)) { }

                //! copy
                inline  tagged_real(const tagged_real &t) throw() : value(t.value), exponent(t.exponent) { }

                //! cleanup
                inline ~tagged_real() throw() {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! '<' operator for odereder list
                inline friend bool operator<(const tagged_real &lhs,
                                             const tagged_real &rhs) throw() {
                    return lhs.exponent < rhs.exponent;
                }

                //! display
                inline friend std::ostream & operator<<(std::ostream &os, const tagged_real &t) {
                    os << t.value << '[' << t.exponent << ']';
                    return os;
                }

                //! access value
                inline const T & operator*() const throw() { return value; }

            private:
                YACK_DISABLE_ASSIGN(tagged_real);
                const T   value;
                static inline int rexp(const T x) throw() {
                    int ex = 0; (void) std::frexp(x,&ex); return ex;
                }

            public:
                const int exponent; //!< exponent value

            };

        }

        //! multiplication info
        class multiplication
        {
        public:

            //! result
            enum result
            {
                regular,   //!< regular output
                underflow, //!< found underflow
                overflow   //!< found overflow
            };

            explicit multiplication() throw(); //!< setup
            virtual ~multiplication() throw(); //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(multiplication);
        };

        //______________________________________________________________________
        //
        //
        //! balancing multiplier
        //
        //______________________________________________________________________
        template <typename T>
        class multiplier : public multiplication, public sorted_list< core::tagged_real<T> >
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef sorted_list<  core::tagged_real<T> >  self_type; //!< alias
            typedef typename self_type::const_type        data_type; //!< alias
            static  const int                             min_exp;   //!< numeric<T>::min_exp
            static  const int                             max_exp;   //!< numeric<T>::max_exp

            //__________________________________________________________________
            //
            // using...
            //__________________________________________________________________
            using self_type::insert;
            using self_type::size;
            using self_type::pull_front;
            using self_type::pull_back;
            using self_type::free;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            inline virtual ~multiplier() throw() {}

            //! setup emptu
            inline explicit multiplier() throw() : multiplication(), self_type() {}

            //! setup with capacity
            inline explicit multiplier(const size_t n) throw() : multiplication(), self_type(n) {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! initialize
            inline void init(const T x) { free(); ld(x); }

            //! initialize with 1
            inline void set1() throw() { free(); }

            //! syntax helper to multiply by a value
            multiplier & operator *=(const T x) { ld(x); return *this; }

            //! syntax helper to initialize with a value
            multiplier & operator =(const T x) { init(x); return *this; }

            //! load a new real
            inline void ld(const T x) { data_type args(x); insert(args); }

            //! load a new real n times a.k.a x^n
            inline void upower(const T x, const size_t n) {
                switch(n)
                {
                    case 0: // assume x!=0...
                        break;
                    default: {
                        const data_type args(x); insert(args,n);
                    } break;
                }
            }

            //! signed power
            inline void ipower(const T x, const int n ) {

                if(n>0)
                {
                    const data_type args(x); insert(args,n);
                }
                else
                {
                    if(n<0) { const data_type args(1.0/x); insert(args,-n); }
                }
            }
            

            //! algorithm: return 1 if empty
            inline T query() { return ( size()  <= 0 ) ? T(1) : query_(); }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(multiplier);
            inline T query_()
            {
                assert( size() >= 1);
                while( size() > 1 ) {
                    const T   lhs( *(pull_front()) );
                    const T   rhs( *(pull_back() ) );
                    const T   ans( lhs * rhs );
                    data_type tmp(ans);
                    insert(tmp);
                }
                return *(pull_front());
            }
        };
    }
}

#endif

