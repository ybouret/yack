//! \file

#ifndef YACK_PCONTOUR_INCLUDED
#define YACK_PCONTOUR_INCLUDED 1

#include "yack/sequence/cxx-array.hpp"
#include "yack/type/v2d.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/spot-object.hpp"
#include "yack/math/numeric.hpp"
#include "yack/sort/heap.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/ios/ocstream.hpp"

namespace yack
{
    namespace math
    {

        namespace kernel
        {
            class periodic_contour : public spot_object
            {
            public:
                typedef memory::dyadic allocator;
                virtual ~periodic_contour() noexcept;

            protected:
                explicit periodic_contour(const size_t n);
                void     raise_singular() const;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(periodic_contour);
            };
        }

        template <typename T>
        class periodic_contour :
        public kernel::periodic_contour,
        public cxx_array< v2d<T>, kernel::periodic_contour::allocator>
        {
        public:
            typedef v2d<T>                      vertex;
            typedef cxx_array<vertex,allocator> vertices;
            typedef cxx_array<T,allocator>      rvector;

            using vertices::size;

            inline explicit periodic_contour(const size_t n) :
            kernel::periodic_contour(n),
            vertices(n),
            bar(),
            theta(n),
            theta_prev(0),
            theta_next(0),
            accel(n),
            speed(n),
            amax(0),
            sm(n,n),
            lu(n),
            rhs(n)
            {
            }

            inline virtual ~periodic_contour() noexcept {}

            inline void update()
            {
                compute_bar();
                build_theta();
                make_spline();
            }

            template <typename FILENAME>
            inline void save(const FILENAME &fn) const
            {
                const vertices &self = *this;
                const size_t    n    =  size();
                ios::ocstream   fp(fn);
                for(size_t i=1;i<=n;++i)
                    emit(fp,self[i]);
                emit(fp,self[1]);
                fp << '\n';
                for(size_t i=1;i<=n;++i)
                {
                    emit(fp,bar);
                    emit(fp,self[i]);
                    fp << '\n';
                }
            }

            template <typename FILENAME>
            inline void save_accel(const FILENAME &fn) const
            {
                const vertices &self = *this;
                const size_t    n    =  size();
                ios::ocstream   fp(fn);
                for(size_t i=1;i<=n;++i)
                {
                    vertex p = self[i];
                    emit(fp,p);
                    p += accel[i]/amax;
                    emit(fp,p);
                    fp << '\n';
                }
            }

            const vertex   bar;    //!< barycenter
            const rvector  theta;
            const double   theta_prev;
            const double   theta_next;
            const vertices accel;
            const vertices speed;
            const double   amax;

        private:
            YACK_DISABLE_ASSIGN(periodic_contour);
            matrix<double> sm;
            crout<double>  lu;
            rvector        rhs;
            static inline void emit(ios::ostream &fp,
                                    const vertex &p)
            {
                fp("%.15g %.15g\n", double(p.x), double(p.y));
            }

            void make_spline()
            {
                // build matrix
                const size_t n = size();
                sm[1][1] = (theta[2] - theta_prev)/3;
                sm[1][2] = (theta[2] - theta[1]  )/6;
                sm[1][n] = (theta[1] - theta_prev)/6;

                for(size_t i=2;i<n;++i)
                {
                    sm[i][i-1] = ( theta[i]   - theta[i-1] )/6;
                    sm[i][i]   = ( theta[i+1] - theta[i-1] )/3;
                    sm[i][i+1] = ( theta[i+1] - theta[i]   )/6;
                }

                sm[n][n]   = ( theta_next - theta[n-1] )/3;
                sm[n][n-1] = ( theta[n]   - theta[n-1] )/6;
                sm[n][1]   = ( theta_next - theta[n]   )/6;

                // decompose matrix
                if(!lu.build(sm)) raise_singular();

                // compute d2p
                const vertices &self = *this;
                for(size_t j=1;j<=2;++j)
                {
                    rhs[1] = (self[2][j]-self[1][j])/(theta[2]-theta[1]) - (self[1][j]-self[n][j])/(theta[1]-theta_prev);
                    for(size_t i=2;i<n;++i)
                    {
                        rhs[i] = (self[i+1][j]-self[i][j])/(theta[i+1]-theta[i]) - (self[i][j]-self[i-1][j])/(theta[i]-theta[i-1]);
                    }
                    rhs[n] = (self[1][j]-self[n][j])/(theta_next-theta[n]) - (self[n][j]-self[n-1][j])/(theta[n]-theta[n-1]);
                    lu.solve(sm,rhs);
                    for(size_t i=n;i>0;--i) coerce(accel[i][j]) = rhs[i];
                }
                std::cerr << "accel = " << accel << std::endl;
                coerce(amax) = 0;
                for(size_t i=n;i>0;--i)
                {
                    const double atmp = accel[i].norm2();
                    if(atmp>amax) coerce(amax) = atmp;
                }
                coerce(amax) = std::sqrt( amax );
            }

            // build theta and order points
            inline void build_theta() noexcept
            {
                static const T  two_pi = numeric<double>::two_pi;
                const size_t    n = size();
                const vertices &p = *this;
                for(size_t i=n;i>0;--i)
                {
                    const T a = (p[i]-bar).angle();
                    coerce(theta[i]) = a >=0 ?  a : (a+two_pi);
                }
                hsort(coerce(theta),coerce(*this), comparison::increasing<T> );
                coerce(theta_prev) = theta[n] - two_pi;
                coerce(theta_next) = theta[1] + two_pi;
            }

            // compute barycenter
            inline void compute_bar() noexcept
            {
                const size_t    n = size();
                const vertices &p = *this;
                coerce(bar)       = vertex(0,0);
                for(size_t i=n;i>0;--i)
                {
                    coerce(bar) += p[i];
                }
                coerce(bar) /= n;
            }
        };

    }
}

#endif

