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
                const int exponent;

                static inline int rexp(const T x) throw() {
                    int ex = 0; (void) std::frexp(x,&ex); return ex;
                }


            };

        }


        //______________________________________________________________________
        //
        //
        //! balancing multiplier
        //
        //______________________________________________________________________
        template <typename T>
        class multiplier : public ordered_list< core::tagged_real<T> >
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef ordered_list<  core::tagged_real<T> >  self_type; //!< alias
            typedef typename self_type::const_type         data_type; //!< alias
            using self_type::insert;
            using self_type::size;
            using self_type::pull_front;
            using self_type::pull_back;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline virtual ~multiplier() throw() {}                               //!< cleanup
            inline explicit multiplier() throw() : self_type() {}                 //!< setup empty
            inline explicit multiplier(const size_t n) throw() : self_type(n) {}  //!< setup with capacity

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! push a new real
            inline void push(const T x) {
                data_type args(x);
                insert(args);
            }

            //! syntax helper
            inline multiplier & operator<<(const T x) { push(x); return *this; }

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

