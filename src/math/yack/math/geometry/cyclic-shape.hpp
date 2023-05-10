
//! \file

#ifndef YACK_GEOMETRIC_CYCLIC_SHAPE_INCLUDED
#define YACK_GEOMETRIC_CYCLIC_SHAPE_INCLUDED 1

#include "yack/data/clist/cxx.hpp"
#include "yack/type/v2d.hpp"
#include "yack/object.hpp"

namespace yack
{
    namespace math
    {

        template <typename T>
        class node2D : public object
        {
        public:
            typedef v2d<T> vertex;

            inline node2D(const T x,
                          const T y) :
            object(), r(x,y), next(0), prev(0)
            {
            }

            inline virtual ~node2D() noexcept {}

            inline node2D(const node2D &other) noexcept :
            object(), r(other.r), next(0), prev(0)
            {
            }


            vertex  r;
            node2D *next;
            node2D *prev;

        private:
            YACK_DISABLE_ASSIGN(node2D);
        };

        template <typename T>
        class cyclic_contour : public cxx_clist_of< node2D<T> >
        {
        public:
            typedef node2D<T> node_type;
            typedef v2d<T>    vertex;
            typedef cxx_clist_of<node_type> nodes;

            explicit cyclic_contour() noexcept : nodes() {}
            virtual ~cyclic_contour() noexcept {}

        private:
            YACK_DISABLE_ASSIGN(cyclic_contour);
        };

    }
}

#endif

