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
            template <typename T>
            class tagged_real
            {
            public:
                const T   value;
                const int exponent;

                inline   tagged_real(const T args) throw() :
                value(args),
                exponent(get_exp(value))
                {
                }

                inline ~tagged_real() throw() {}
                inline  tagged_real(const tagged_real &t) throw() :
                value(t.value),
                exponent(t.exponent)
                {
                }

                inline friend bool operator<(const tagged_real &lhs,
                                             const tagged_real &rhs) throw()
                {
                    return lhs.exponent < rhs.exponent;
                }

                inline friend std::ostream & operator<<(std::ostream &os, const tagged_real &t)
                {
                    os << t.value << '[' << t.exponent << ']';
                    return os;
                }

            private:
                YACK_DISABLE_ASSIGN(tagged_real);

                static inline int get_exp(const T x) throw()
                {
                    int ex = 0;
                    (void) std::frexp(x,&ex);
                    return ex;
                }
            };

        }

        template <typename T>
        class multiplier : public ordered_list< core::tagged_real<T> >
        {
        public:
            typedef ordered_list<  core::tagged_real<T> >  self_type;
            typedef typename self_type::const_type         data_type;
            using self_type::insert;

            inline virtual ~multiplier() throw() {}
            inline explicit multiplier() throw() : self_type() {}
            inline explicit multiplier(const size_t n) throw() : self_type(n) {}

            inline void push(const T x)
            {
                data_type args(x);
                insert(args);
            }

            inline multiplier & operator<<(const T x)
            {
                push(x);
                return *this;
            }

            inline T query()
            {
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(multiplier);

        };
    }
}

#endif

