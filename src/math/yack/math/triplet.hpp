//! \file

#ifndef YACK_TRIPLET_INCLUDED
#define YACK_TRIPLET_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include <iostream>

namespace yack
{

    namespace math
    {

        template <typename T>
        struct triplet {
            YACK_DECL_ARGS_(T,type);
            T a;
            T b;
            T c;

            inline size_t size() const throw() { return 3; }
            inline type  &operator[](const size_t indx) throw()
            {
                assert(indx>=1); assert(indx<=3);
                return *((((mutable_type *)&a)-1)+indx);
            }

            inline const_type &operator[](const size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=3);
                return *((((const_type *)&a)-1)+indx);
            }

            inline void sort() throw()
            {
                hsort(*this,comparison::increasing<T>);
            }

            inline bool is_increasing() const throw()
            {
                return comparison::ordered(&a,3,comparison::increasing<T>);
            }

            friend inline
            std::ostream & operator<<(std::ostream &os, const triplet &t)
            {
                os << '[' << t.a << ';' << t.b << ';' << t.c << ']';
                return os;
            }


        };

    }

}

#endif

