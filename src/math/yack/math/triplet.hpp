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

        //______________________________________________________________________
        //
        //
        //! multipurpose triplet
        //
        //______________________________________________________________________
        template <typename T>
        struct triplet
        {

            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            YACK_DECL_ARGS_(T,type); //!< aliases


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            T a; //!< value
            T b; //!< value
            T c; //!< value

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! array like interface
            inline size_t size() const throw() { return 3; }

            //! array like interface
            inline type  &operator[](const size_t indx) throw()
            {
                assert(indx>=1); assert(indx<=3);
                return *((((mutable_type *)&a)-1)+indx);
            }

            //! array like interface
            inline const_type &operator[](const size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=3);
                return *((((const_type *)&a)-1)+indx);
            }

            //! sort in increasing order
            inline void sort() throw()
            {
                hsort(*this,comparison::increasing<T>);
            }

            //! check if is increasing
            inline bool is_increasing() const throw()
            {
                return comparison::ordered(&a,3,comparison::increasing<T>);
            }

            //! check if is decreasing
            inline bool is_decreasing() const throw()
            {
                return comparison::ordered(&a,3,comparison::decreasing<T>);
            }

            //! check if is ordered
            inline bool is_ordered() const throw()
            {
                return is_increasing() || is_decreasing();
            }


            //! output as a 3-array
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

