
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

        //______________________________________________________________________
        //
        //
        //! a node in 2D with its components
        //
        //______________________________________________________________________
        template <typename T>
        class node2D : public spot_object
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef v2d<T> vertex; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline node2D(const vertex p) :
            spot_object(), r(p), speed(), accel(), t(), n(), kappa(0), next(0), prev(0)
            {
            }

            //! cleanup
            inline virtual ~node2D() noexcept {}

            //! copy
            inline node2D(const node2D &_) noexcept :
            spot_object(), r(_.r), speed(_.speed), accel(_.accel), t(_.t), n(_.n), kappa(_.kappa), next(0), prev(0)
            {
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display as position
            inline friend std::ostream & operator<<(std::ostream &os, const node2D &node)
            {
                os << node.r;
                return os;
            }

            
            inline vertex       & operator*() noexcept       { return r; } //!< access
            inline const vertex & operator*() const noexcept { return r; } //!< access



            //! update metrics
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
                const T      a  = hypothenuse(AB.x,AB.y);
                const T      b  = hypothenuse(BC.x,BC.y);
                const T      c  = hypothenuse(AC.x,AC.y);
                const T      s  = half * (a+b+c);
                const T      area = std::sqrt( s * (s-a) * (s-b) * (s-c) );

                kappa = four * area /a/b/c;
                speed = half * AC;
                t     =  speed / hypothenuse(speed.x,speed.y);
                n.x   =  t.y;
                n.y   = -t.x;
                accel = BC-AB;
                if(  accel *  n > 0) kappa = -kappa;
                //std::cerr << "kappa=" << kappa << std::endl;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            vertex  r;      //!< position
            vertex  speed;  //!< speed
            vertex  accel;  //!< acceleration
            vertex  t;      //!< tangent vector
            vertex  n;      //!< normal vector
            T       kappa;  //!< curvature
            node2D *next;   //!< for list
            node2D *prev;   //!< for list

        private:
            YACK_DISABLE_ASSIGN(node2D);
        };


        //______________________________________________________________________
        //
        //
        //! a segment based on two nodes2D
        //
        //______________________________________________________________________
        template <typename T>
        class segment2D : public spot_object
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef node2D<T> node_type; //!< alias
            typedef v2d<T>    vertex;    //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit segment2D(const node_type *a, const node_type *b) noexcept :
            spot_object(), A(a), B(b), quadratic(), cubic(), next(0), prev(0)
            {
                assert(a!=b);
            }

            //! cleanup
            inline virtual ~segment2D() noexcept {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! update metrics
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


            //! evaluate continuous contour between A and B
            inline vertex eval(const double lam) const
            {
                return A->r + lam * ( A->speed + lam * ( quadratic + lam * cubic) );
            }

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const segment2D &seg)
            {
                os << '{' << *seg.A << "->" << *seg.B << '}';
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const node_type * const A;         //!< first node
            const node_type * const B;         //!< second node
            vertex                  quadratic; //!< quadratic term
            vertex                  cubic;     //!< cubic term
            segment2D       *       next;      //!< for list
            segment2D       *       prev;      //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(segment2D);
        };


        //______________________________________________________________________
        //
        //
        //! cyclic 2D contour
        //
        //______________________________________________________________________
        template <typename T>
        class cyclic_contour : public cxx_clist_of< node2D<T> >
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef node2D<T>            node_t;   //!< alias
            typedef v2d<T>               vertex;   //!< alias
            typedef segment2D<T>         segm_t;   //!< alias
            typedef cxx_clist_of<node_t> nodes;    //!< alias
            typedef cxx_clist_of<segm_t> segments; //!< alias

            using nodes::head;
            using nodes::size;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit cyclic_contour() noexcept : nodes(), sides(), bar() {}

            //! cleanup
            virtual ~cyclic_contour() noexcept {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! append a new vertex
            cyclic_contour & operator<<( const vertex p )
            {
                this->push_back( new node_t(p) );
                return *this;
            }

            //! save vertices
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

            //! save acceleration
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


            //! save tangent
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

            //! save smooth shape with points per side
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


            //! save normals
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

            //! save curvature using normals
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


            //! full metrics update
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

            //! translate barycenter to origin
            void center() noexcept
            {
                node_t *node = this->head;
                for(size_t i=size;i>0;--i,node=node->next)
                {
                    node->r -= bar;
                }
                bar             = vertex(0,0);
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            segments sides; //!< sides for nodes
            vertex   bar;   //!< barycenter

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

