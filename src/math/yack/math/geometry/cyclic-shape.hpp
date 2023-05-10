
//! \file

#ifndef YACK_GEOMETRIC_CYCLIC_SHAPE_INCLUDED
#define YACK_GEOMETRIC_CYCLIC_SHAPE_INCLUDED 1

#include "yack/data/clist/cxx.hpp"
#include "yack/type/v2d.hpp"
#include "yack/object.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/math/numeric.hpp"
#include "yack/spot-object.hpp"

namespace yack
{
    namespace math
    {

        template <typename T>
        class node2D : public spot_object
        {
        public:
            typedef v2d<T> vertex;

            inline node2D(const vertex p) :
            spot_object(), r(p), v(), a(), t(), n(), next(0), prev(0)
            {
            }

            inline virtual ~node2D() noexcept {}

            inline node2D(const node2D &_) noexcept :
            spot_object(), r(_.r), v(_.v), a(_.a), t(_.t), n(_.n), next(0), prev(0)
            {
            }




            vertex  r;
            vertex  v;
            vertex  a;
            vertex  t;
            vertex  n;
            T       kappa;
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
            using nodes::head;
            using nodes::size;

            explicit cyclic_contour() noexcept : nodes() {}
            virtual ~cyclic_contour() noexcept {}

            cyclic_contour & operator<<( const vertex p )
            {
                this->push_back( new node_type(p) );
                return *this;
            }

            template <typename FILENAME> inline
            void save(const FILENAME &fn)
            {
                ios::ocstream   fp(fn);
                const node_type *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    emit(fp,node->r);
                }
                emit(fp,node->r);
            }


            void update()
            {
                node_type *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    static const T half(0.5);
                    static const T four(4);
                    const vertex A = node->prev->r;
                    const vertex B = node->r;
                    const vertex C = node->next->r;
                    const vertex AB = B-A;
                    const vertex BC = C-B;
                    const vertex AC = C-A;
                    const T      a  = hypothenuse(AB);
                    const T      b  = hypothenuse(BC);
                    const T      c  = hypothenuse(AC);
                    const T      s  = half * (a+b+c);
                    const T      area = std::sqrt( s * (s-a) * (s-b) * (s-c) );
                    node->kappa = four * area /a/b/c;
                    std::cerr << "kappa=" << node->kappa << std::endl;
                    node->v     = half * AC;

                }
            }

        private:
            YACK_DISABLE_ASSIGN(cyclic_contour);
            static inline void emit(ios::ocstream &fp,
                                    const vertex  &v)
            {
                fp("%.15g %.15g\n",double(v.x),double(v.y));
            }
        };

    }
}

#endif

