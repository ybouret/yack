
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
            spot_object(), r(p), speed(), accel(), t(), n(), kappa(0), next(0), prev(0)
            {
            }

            inline virtual ~node2D() noexcept {}

            inline node2D(const node2D &_) noexcept :
            spot_object(), r(_.r), speed(_.speed), accel(_.accel), t(_.t), n(_.n), kappa(_.kappa), next(0), prev(0)
            {
            }

            inline friend std::ostream & operator<<(std::ostream &os, const node2D &node)
            {
                os << node.r;
                return os;
            }

            inline vertex       & operator*() noexcept       { return r; }
            inline const vertex & operator*() const noexcept { return r; }


            inline void update()
            {
                static const T half(0.5);
                static const T four(4);

                const vertex A  = prev->r;
                const vertex B  = r;
                const vertex C  = next->r;
                const vertex AB = B-A;
                const vertex BC = C-B;
                const vertex AC = C-A;
                const T      a  = hypothenuse(AB);
                const T      b  = hypothenuse(BC);
                const T      c  = hypothenuse(AC);
                const T      s  = half * (a+b+c);
                const T      area = std::sqrt( s * (s-a) * (s-b) * (s-c) );

                kappa = four * area /a/b/c;
                speed = half * AC;
                t     =  speed / hypothenuse(speed);
                n.x   =  t.y;
                n.y   = -t.x;
                accel = BC-AB;
                if(  accel *  n > 0) kappa = -kappa;
                //std::cerr << "kappa=" << kappa << std::endl;
            }


            vertex  r;
            vertex  speed;
            vertex  accel;
            vertex  t;
            vertex  n;
            T       kappa;
            node2D *next;
            node2D *prev;

        private:
            YACK_DISABLE_ASSIGN(node2D);
        };

        template <typename T>
        class segment2D : public spot_object
        {
        public:
            typedef node2D<T> node_type;
            typedef v2d<T>    vertex;

            inline explicit segment2D(const node_type *a, const node_type *b) noexcept :
            spot_object(), A(a), B(b), quadratic(), cubic(), next(0), prev(0)
            {
            }

            inline virtual ~segment2D() noexcept {}

            inline void update()
            {
                const vertex AB   = B->r - A->r;
                const vertex vA   = A->speed;
                const vertex vB   = B->speed;
                const vertex rhs1 = AB - vA;
                const vertex rhs2 = vB - vA;
                quadratic = 3*rhs1 - rhs2;
                cubic     = rhs2 - 2*rhs1;
            }

            inline vertex eval(const double lam) const
            {
                return A->r + lam * ( A->speed + lam * ( quadratic + lam * cubic) );
            }

            inline friend std::ostream & operator<<(std::ostream &os, const segment2D &seg)
            {
                os << '{' << *seg.A << "->" << *seg.B << '}';
                return os;
            }

            const node_type * const A;
            const node_type * const B;
            vertex                  quadratic;
            vertex                  cubic;
            segment2D       *       next;
            segment2D       *       prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(segment2D);
        };

        template <typename T>
        class cyclic_contour : public cxx_clist_of< node2D<T> >
        {
        public:
            typedef node2D<T>            node_t;
            typedef v2d<T>               vertex;
            typedef segment2D<T>         segm_t;
            typedef cxx_clist_of<node_t> nodes;
            typedef cxx_clist_of<segm_t> segments;

            using nodes::head;
            using nodes::size;

            explicit cyclic_contour() noexcept : nodes(), sides(), bar() {}
            virtual ~cyclic_contour() noexcept {}

            cyclic_contour & operator<<( const vertex p )
            {
                this->push_back( new node_t(p) );
                return *this;
            }

            template <typename FILENAME> inline
            void save(const FILENAME &fn)
            {
                ios::ocstream fp(fn);
                const node_t *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    emit(fp,node->r);
                }
                emit(fp,node->r);
            }

            template <typename FILENAME> inline
            void save_a(const FILENAME &fn)
            {
                ios::ocstream   fp(fn);
                const node_t *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    emit(fp,node->r);
                    emit(fp,node->r+node->accel);
                    fp << '\n';
                }
            }

            template <typename FILENAME> inline
            void save_t(const FILENAME &fn)
            {
                ios::ocstream fp(fn);
                const node_t *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    emit(fp,node->r);
                    emit(fp,node->r+node->t);
                    fp << '\n';
                }
            }

            template <typename FILENAME> inline
            void smooth(const FILENAME &fn, const size_t pps)
            {
                ios::ocstream  fp(fn);
                const segm_t  *seg = sides.head;
                for(size_t i=sides.size;i>0;--i, seg=seg->next)
                {
                    for(size_t i=0;i<pps;++i)
                    {
                        const T l0 = T(i)/pps;
                        emit(fp,seg->eval(l0));
                    }
                }
                emit(fp,sides.head->eval(0));
            }


            template <typename FILENAME> inline
            void save_n(const FILENAME &fn)
            {
                ios::ocstream fp(fn);
                const node_t *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    emit(fp,node->r);
                    emit(fp,node->r+node->n);
                    fp << '\n';
                }
            }

            template <typename FILENAME> inline
            void save_kappa(const FILENAME &fn)
            {
                ios::ocstream fp(fn);
                const node_t *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    emit(fp,node->r);
                    emit(fp,node->r+node->kappa*node->n);
                    fp << '\n';
                }
            }



            void update()
            {
                sides.release();
                bar          = vertex(0,0);
                {
                    node_t *node = this->head;
                    for(size_t i=size;i>0;--i,node=node->next)
                    {
                        node->update();
                        bar += node->r;
                        sides.push_back( new segm_t(node,node->next) );
                    }
                }
                bar /= size;
                {
                    segm_t  *seg = sides.head;
                    for(size_t i=sides.size;i>0;--i, seg=seg->next)
                    {
                        seg->update();
                    }
                }
            }

            void center() noexcept
            {
                node_t *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    node->r -= bar;
                }
                bar             = vertex(0,0);
            }

            segments sides;
            vertex   bar;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cyclic_contour);



            static inline void emit(ios::ocstream &fp,
                                    const vertex  &v)
            {
                fp("%.15g %.15g\n",double(v.x),double(v.y));
            }
        };

    }
}

#endif

