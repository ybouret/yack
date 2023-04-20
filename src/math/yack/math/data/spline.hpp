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
            typedef tridiag<abscissa>             straight_solver;
            typedef cyclic<abscissa>              periodic_solver;

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




            void add(const param_abscissa X,
                     const param_ordinate Y)
            {
                coerce(x).push_back(X);
                try { coerce(y).push_back(Y);  } catch(...) { coerce(x).pop_back(); throw; }
                assert(x.size()==y.size());
                try { coerce(y2).push_back(Y); } catch(...) { coerce(x).pop_back(); coerce(y).pop_back(); throw; }
                assert(x.size()==y2.size());
                out_of_reach::zset(&coerce(y2.back()),sizeof(ordinate));
            }

            virtual ordinate        operator()(param_abscissa xx) const = 0;
            virtual const_abscissa &lower() const noexcept = 0;
            virtual const_abscissa &upper() const noexcept = 0;
            virtual bool            scale() = 0;

            const abscissae x;
            const ordinates y;
            const ordinates y2;

            class common
            {
            public:
                inline virtual ~common() noexcept {}

            protected:
                inline explicit common(const size_t n) :
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
                YACK_DISABLE_COPY_AND_ASSIGN(common);
            };

            class straight : public straight_solver, public common
            {
            public:
                using straight_solver::a;
                using straight_solver::b;
                using straight_solver::c;
                using common::r;
                using common::u;
                using common::h;



                inline explicit straight(const size_t n) :
                straight_solver(n),
                common(n)
                {
                }

                inline virtual ~straight() noexcept
                {

                }

                //! natural spline
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

                    // finalize bulk
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
                YACK_DISABLE_COPY_AND_ASSIGN(straight);
            };

            class periodic : public periodic_solver, public common
            {
            public:
                using periodic_solver::a;
                using periodic_solver::b;
                using periodic_solver::c;
                using common::r;
                using common::u;
                using common::h;


                inline explicit periodic(const size_t n) :
                periodic_solver(n),
                common(n)
                {
                }

                inline virtual ~periodic() noexcept
                {

                }

                bool operator()(spline        &s,
                                const abscissa x_prev,
                                const abscissa x_next)
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
                    a[1] = (s.x[1]-x_prev)/6;
                    b[1] = (s.x[2]-x_prev)/3;
                    c[1] = (s.x[2]-s.x[1])/6;

                    a[n] = (s.x[n]-s.x[n-1])/6;
                    b[n] = (x_next-s.x[n-1])/3;
                    c[n] = (x_next-s.x[n])/6;

                    //std::cerr << "M=" << (*this) << std::endl;

                    for(unsigned d=0;d<dimension;++d)
                    {
                        this->bulk(s,d);
                        r[1] = (coord(s.y[2],d)-coord(s.y[1],d))/(s.x[2]-s.x[1])
                        - (coord(s.y[1],d)-coord(s.y[n],d))/(s.x[1]-x_prev);

                        r[n] = ( coord(s.y[1],d)-coord(s.y[n],d))/(x_next-s.x[n])
                        - ( coord(s.y[n],d)-coord(s.y[n-1],d))/(s.x[n]-s.x[n-1]);

                        if(!this->solve(u,r)) return false;
                        for(size_t i=n;i>0;--i)
                        {
                            coerce(coord(s.y2[i],d)) = u[i];
                        }
                    }

                    return true;
                }
                

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(periodic);
            };



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(spline);


        };

        template <typename ABSCISSA, typename ORDINATE>
        class straight_spline : public spline<ABSCISSA,ORDINATE>
        {
        public:
            typedef spline<ABSCISSA,ORDINATE>      spline_type;
            typedef typename spline_type::straight solver_type;
            YACK_DECL_ARGS(ABSCISSA,abscissa);
            YACK_DECL_ARGS(ORDINATE,ordinate);
            using spline_type::x;
            using spline_type::y;
            using spline_type::y2;



            inline virtual ~straight_spline() noexcept {}
            inline explicit straight_spline(const size_t n) :
            spline_type(n)
            {}

            inline bool build_with( solver_type &solver )
            {
                return solver(*this);
            }

            inline bool build()
            {
                solver_type solver( this->size() );
                return build_with(solver);
            }

            virtual const_abscissa &lower() const noexcept
            {
                assert(this->size()>0);
                return x.front();
            }

            virtual const_abscissa &upper() const noexcept
            {
                assert(this->size()>0);
                return x.back();
            }

            virtual bool scale()
            {
                const size_t n = this->size();
                if(n>=2)
                {
                    static const_abscissa _1(1);
                    mutable_abscissa xx = 0;
                    for(size_t i=1;i<=n;++i,xx+=_1)
                    {
                        coerce(x[i]) = xx;
                    }
                    return build();
                }
                return true;
            }


            virtual ordinate operator()(param_abscissa xx) const
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


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(straight_spline);
        };

        template <typename ABSCISSA, typename ORDINATE>
        class periodic_spline : public spline<ABSCISSA,ORDINATE>
        {
        public:
            typedef spline<ABSCISSA,ORDINATE>      spline_type;
            typedef typename spline_type::periodic solver_type;

            YACK_DECL_ARGS(ABSCISSA,abscissa);
            YACK_DECL_ARGS(ORDINATE,ordinate);
            
            using spline_type::x;
            using spline_type::y;
            using spline_type::y2;

            const_abscissa xlower; //!< x[1]
            const_abscissa xupper; //!< from x_next
            const_abscissa period; //!< xupper - xlower

            inline virtual ~periodic_spline() noexcept {}
            inline explicit periodic_spline(const size_t n) :
            spline_type(n),
            xlower(0),
            xupper(0),
            period(0)
            {}

            inline bool build_with(solver_type   &solver,
                                   param_abscissa x_prev,
                                   param_abscissa x_next)
            {
                coerce(xlower) = 0;
                coerce(xupper) = 0;
                coerce(period) = 0;
                if( solver(*this,x_prev,x_next) )
                {
                    if(this->size()>=2)
                    {
                        coerce(xlower) = x[1];
                        coerce(xupper) = x_next;
                        coerce(period) = xupper-xlower;
                    }
                    return true;
                }
                else
                {

                    return false;
                }
            }

            inline bool build(param_abscissa x_prev,
                              param_abscissa x_next)
            {
                solver_type solver(this->size());
                return build_with(solver,x_prev,x_next);
            }

            virtual const_abscissa &lower() const noexcept
            {
                return xlower;
            }

            virtual const_abscissa &upper() const noexcept
            {
                return xupper;
            }

            virtual bool scale()
            {
                const size_t n = this->size();
                if(n>=2)
                {
                    static const_abscissa _1(1);
                    mutable_abscissa xx = 0;
                    for(size_t i=1;i<=n;++i,xx+=_1)
                    {
                        coerce(x[i]) = xx;
                    }
                    return build(-_1,xx);
                }
                return true;
            }

            virtual ordinate operator()(param_abscissa X) const
            {
                const size_t n = x.size(); assert(n>0);

                if(n<=1)
                {
                    return y[1];
                }
                else
                {
                    mutable_abscissa xx = X;
                    while(xx<xlower) xx += period;
                    while(xx>xupper) xx -= period;
                    const_abscissa   xend = x[n];

                    size_t           jlo=1;
                    size_t           jup=2;
                    mutable_abscissa xlo=0;
                    mutable_abscissa xup=0;
                    if(xx>=xend)
                    {
                        //std::cerr << "in extra" << std::endl;
                        jlo=n;
                        jup=1;
                        xlo=xend;
                        xup=xupper;
                    }
                    else
                    {
                    CHECK:
                        if(xx>=x[jlo]&&xx<=x[jup]) goto FOUND;
                        ++jlo;
                        ++jup;
                        goto CHECK;
                    FOUND:
                        xlo = x[jlo];
                        xup = x[jup];
                    }
                    static const_abscissa _0(0);
                    static const_abscissa _1(1);

                    const_abscissa d = xup - xx;
                    const_abscissa h = xup - xlo; assert(h>0);
                    const_abscissa A   = (d<=_0) ? _0 :(  (d>=h) ? _1 : d/h);
                    const_abscissa B   = _1-A;

                    return
                    A * y[jlo] + B * y[jup] +
                    (A*(A*A-_1)*y2[jlo]+B*(B*B-_1)*y2[jup]) * h * h / 6;
                }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(periodic_spline);
        };


    }
}

#endif

