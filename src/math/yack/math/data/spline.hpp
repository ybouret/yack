//! \file

#ifndef YACK_SPLINE_INCLUDED
#define YACK_SPLINE_INCLUDED 1

#include "yack/math/api.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/math/algebra/cyclic.hpp"
#include "yack/sort/heap.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace math
    {
        class spline_
        {
        public:
            typedef memory::dyadic allocator;

            explicit spline_() noexcept {}
            virtual ~spline_() noexcept {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(spline_);
        };

        template <typename ABSCISSA, typename ORDINATE>
        class spline : public spline_
        {
        public:
            YACK_DECL_ARGS(ABSCISSA,abscissa);
            YACK_DECL_ARGS(ORDINATE,ordinate);
            typedef vector<abscissa,allocator>    abscissae;
            typedef vector<ordinate,allocator>    ordinates;
            static const size_t                   dimension = sizeof(ORDINATE)/sizeof(ABSCISSA);
            typedef cxx_array<abscissa,allocator> tableau;
            typedef tridiag<abscissa>             linear_solver;
            typedef cyclic<abscissa>              cyclic_solver;

            static inline const abscissa &coord(const_ordinate &Y, const unsigned d) noexcept
            {
                const abscissa *p = (const abscissa *)&Y;
                return p[d];
            }

            static inline ios::ostream & print(ios::ostream &fp, param_ordinate Y)
            {
                for(unsigned d=0;d<dimension;++d)
                {
                    fp(" %.15g", double( coord(Y,d) ) );
                }
                return fp;
            }

            inline virtual ~spline() noexcept {}
            inline explicit spline(const size_t n) : spline_(),
            x(n,as_capacity),
            y(n,as_capacity),
            y2(n,as_capacity)
            {
            }

            inline size_t size() const noexcept
            {
                assert(x.size()== y.size());
                assert(x.size()==y2.size());
                return x.size();
            }

            inline void clear() noexcept
            {
                out_of_reach::zset( coerce(y2)(), y2.size()*sizeof(ordinate) );
            }

            inline void sort() noexcept
            {
                hsort(coerce(x),coerce(y),comparison::increasing<const_abscissa>);
            }


            void operator()(const param_abscissa X,
                            const param_ordinate Y)
            {
                coerce(x).push_back(X);
                try { coerce(y).push_back(Y);  } catch(...) { coerce(x).pop_back(); throw; }
                assert(x.size()==y.size());
                try { coerce(y2).push_back(Y); } catch(...) { coerce(x).pop_back(); coerce(y).pop_back(); throw; }
                assert(x.size()==y2.size());
                out_of_reach::zset(&coerce(y2.back()),sizeof(ordinate));
            }

            ordinate operator()(param_abscissa xx) const
            {
                const size_t n = x.size(); assert(n>0);
                if(n<=1)
                {
                    return y[1];
                }
                else
                {
                    if(xx<=x[1])
                    {
                        return y[1];
                    }
                    else
                    {
                        if(xx>=x[n])
                        {
                            return y[n];
                        }
                        else
                        {
                            size_t jlo = 1;
                            size_t jup = 2;
                        CHECK:
                            if(xx>=x[jlo]&&xx<=x[jup]) goto FOUND;
                            ++jlo;
                            ++jup;
                            goto CHECK;
                        FOUND:
                            static const_abscissa _0(0);
                            static const_abscissa _1(1);

                            const_abscissa d = x[jup] - xx;
                            const_abscissa h = x[jup] - x[jlo]; assert(h>0);
                            const_abscissa A   = (d<=_0) ? _0 :(  (d>=h) ? _1 : d/h);
                            const_abscissa B   = _1-A;

                            return
                            A * y[jlo] + B * y[jup] +
                            (A*(A*A-_1)*y2[jlo]+B*(B*B-_1)*y2[jup]) * h * h / 6;
                        }
                    }
                }
            }



            const abscissae x;
            const ordinates y;
            const ordinates y2;


            class common_solver
            {
            public:
                inline virtual ~common_solver() noexcept {}

            protected:
                inline explicit common_solver(const size_t n) :
                u(n), r(n), h(n)
                {
                }

                inline void init(writable<abscissa> &a,
                                 writable<abscissa> &b,
                                 writable<abscissa> &c,
                                 const spline       &s)
                {
                    const size_t n = s.size();

                    for(size_t i=2,im=1;i<=n;++i,++im)
                    {
                        h[i] = s.x[i] - s.x[im];
                    }

                    for(size_t i=2;i<n;++i)
                    {
                        a[i] = h[i]/6;
                        b[i] = (s.x[i+1]-s.x[i-1])/3;
                        c[i] = h[i+1]/6;
                    }
                }

                void bulk(const spline &s, const unsigned d)
                {
                    const size_t n = s.size();
                    for(size_t i=2;i<n;++i)
                    {
                        const abscissa ym = coord(s.y[i-1], d);
                        const abscissa y0 = coord(s.y[i],   d);
                        const abscissa yp = coord(s.y[i+1], d);
                        r[i] = (yp-y0)/h[i+1] - (y0-ym)/h[i];
                    }
                }

                tableau u;
                tableau r;
                tableau h;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(common_solver);
            };

            class linear : public linear_solver, public common_solver
            {
            public:
                using linear_solver::a;
                using linear_solver::b;
                using linear_solver::c;
                using common_solver::r;
                using common_solver::u;
                using common_solver::h;


                inline explicit linear(const size_t n) :
                linear_solver(n),
                common_solver(n)
                {
                }

                inline virtual ~linear() noexcept
                {

                }

                bool operator()(spline &s)
                {
                    assert(this->mutual_size()==s.size());
                    const size_t n = s.size();
                    switch(n)
                    {
                        case 0:
                        case 1:
                        case 2:
                            s.clear();
                            return true;
                        default:
                            break;
                    }
                    // prepare matrix
                    this->init(a,b,c,s);

                    // prepare boundary conditions
                    a[1] = 0; b[1] = 1; c[1] = 0; r[1] = 0;
                    a[n] = 0; b[n] = 1; c[n] = 0; r[n] = 0;
                    for(unsigned d=0;d<dimension;++d)
                    {
                        this->bulk(s,d);
                        if(!this->solve(u,r)) return false;
                        for(size_t i=n;i>0;--i)
                        {
                            coerce(coord(s.y2[i],d)) = u[i];
                        }
                    }

                    return true;
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(linear);


            };


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(spline);



        };

        


    }
}

#endif

