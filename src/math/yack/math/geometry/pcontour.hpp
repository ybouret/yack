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
            //__________________________________________________________________
            //
            //! common class for periodic contours
            //__________________________________________________________________
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

        //______________________________________________________________________
        //
        //
        //! periodic contour
        //
        //______________________________________________________________________
        template <typename T>
        class periodic_contour :
        public kernel::periodic_contour,
        public cxx_array< v2d<T>, kernel::periodic_contour::allocator>
        {
        public:
            //__________________________________________________________________
            //
            //! definitions
            //__________________________________________________________________
            typedef v2d<T>                      vertex;   //!< alias
            typedef cxx_array<vertex,allocator> vertices; //!< alias
            typedef cxx_array<T,allocator>      rvector;  //!< alias
            using vertices::size;

            //__________________________________________________________________
            //
            //! C++
            //__________________________________________________________________
            inline explicit periodic_contour(const size_t n) :
            kernel::periodic_contour(n),
            vertices(n),
            bar(),
            theta(n),
            theta_prev(0),
            theta_next(0),
            curv_scale(1),
            accel(n),
            speed(n),
            tvec(n),
            nvec(n),
            curv(n),
            amax(0),
            smax(0),
            cmax(0),
            sm(n,n),
            lu(n),
            rhs(n)
            {
                vertices &self = *this;
                for(size_t i=0;i<n;++i)
                {
                    const double th = (i*numeric<double>::two_pi) / n;
                    self[i+1] = vertex(  cos(th),   sin(th) );
                }
                build_theta();
                make_spline();
                {
                    double kappa = 0;
                    for(size_t i=n;i>0;--i) kappa += (coerce(curv[i]) = approx_curv(i));
                    coerce(curv_scale) = kappa / n;
                }
                for(size_t i=n;i>0;--i) {
                    coerce(curv[i]) /= curv_scale;
                }
            }

            inline virtual ~periodic_contour() noexcept {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! update from current points
            inline void update()
            {
                compute_bar();
                build_theta();
                make_spline();
                local_frame();
            }

            //! save control points
            template <typename FILENAME>
            inline void save(const FILENAME &fn,
                             const bool append = false) const
            {
                const vertices &self = *this;
                const size_t    n    =  size();
                ios::ocstream   fp(fn,append);
                if(append) fp << '\n';
                for(size_t i=1;i<=n;++i)
                    emit(fp,self[i]);
                emit(fp,self[1]);
                fp << '\n';
                if(false)
                {
                    for(size_t i=1;i<=n;++i)
                    {
                        emit(fp,bar);
                        emit(fp,self[i]);
                        fp << '\n';
                    }
                }
            }

            //! save rescaled acceleration at control points
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

            //! save rescaled speeds at control points
            template <typename FILENAME>
            inline void save_speed(const FILENAME &fn) const
            {
                const vertices &self = *this;
                const size_t    n    =  size();
                ios::ocstream   fp(fn);
                for(size_t i=1;i<=n;++i)
                {
                    vertex p = self[i];
                    emit(fp,p);
                    p += speed[i]/smax;
                    emit(fp,p);
                    fp << '\n';
                }
            }

            //! save curvature at control points
            template <typename FILENAME>
            inline void save_curv(const FILENAME &fn) const
            {
                const vertices &self = *this;
                const size_t    n    =  size();
                ios::ocstream   fp(fn);
                for(size_t i=1;i<=n;++i)
                {
                    vertex p = self[i];
                    emit(fp,p);
                    p += curv[i] * nvec[i] / cmax;
                    emit(fp,p);
                    fp << '\n';
                }
            }

            //! using spline interpolation
            vertex operator()(const double th) const
            {
                static const T one(1);
                static const T six(6);
                size_t klo=0,khi=0;
                T  A=0,B=0;
                const double h   = find(klo,khi,A,B,th);
                const vertex lo  = (*this)[klo];
                const vertex hi  = (*this)[khi];
                const vertex d2lo = accel[klo];
                const vertex d2hi = accel[khi];
                const vertex dtmp = A*(A*A-one) * d2lo + B*(B*B-one) * d2hi;
                return A*lo + B*hi +  (h*h*dtmp) / six;
            }

            vertex spd(const double th)
            {
                static const T one(1);
                static const T six(6);
                size_t klo=0,khi=0;
                T      A=0,B=0;
                const T h        = find(klo,khi,A,B,th);
                const vertex lo  = (*this)[klo];
                const vertex hi  = (*this)[khi];
                const vertex d2lo = accel[klo];
                const vertex d2hi = accel[khi];
                return (hi-lo)/h + (((one-3.0*A*A) * d2lo + (3.0*B*B-one) * d2hi)*h)/six;
            }


            //! save shape using np>=3 points
            template <typename FILENAME>
            inline void save_shape(const FILENAME &fn,
                                   const size_t    np,
                                   const bool      append = false) const
            {
                assert(np>=3);
                ios::ocstream fp(fn,append);
                if(append) fp << '\n';
                for(size_t i=0;i<np;++i)
                {
                    const T      th = (i*numeric<T>::two_pi)/(np);
                    const vertex p = (*this)(th);
                    emit(fp,p);
                }
                {
                    const vertex p = (*this)(0);
                    emit(fp,p);
                }

            }

            const vertex   bar;          //!< barycenter
            const rvector  theta;        //!< angles
            const T        theta_prev;   //!< theta[0]
            const T        theta_next;   //!< theta[N+1]
            const T        curv_scale;   //!< for a circle
            const vertices accel;        //!< accelerations
            const vertices speed;        //!< speeds
            const vertices tvec;         //!< tangent vector
            const vertices nvec;         //!< outer normal vector
            const rvector  curv;         //!< curvature
            const double   amax;         //!< max |accel|
            const double   smax;         //!< max |speed|
            const double   cmax;         //!< max |curv|

        private:
            YACK_DISABLE_ASSIGN(periodic_contour);
            matrix<double> sm;
            crout<double>  lu;
            rvector        rhs;


            inline T find(size_t &klo,
                          size_t &khi,
                          T      &A,
                          T      &B,
                          T       th) const
            {
                while(th>numeric<T>::two_pi) th -= numeric<T>::two_pi;
                while(th<0)                  th += numeric<T>::two_pi;

                const size_t N = size();
                if(th<=theta[1])
                {
                    klo = N;
                    khi = 1;
                    const T h = theta[1] - theta_prev;
                    A = clamp<T>(0,(theta[1] - th)/h,1);
                    B = clamp<T>(0,(th-theta_prev)/h,1);
                    return h;
                }
                else
                {
                    if(th>=theta[N])
                    {
                        klo = N;
                        khi = 1;
                        const T h = theta_next - theta[N];
                        A = clamp<T>(0,(theta_next - th)/h,1);
                        B = clamp<T>(0,(th-theta[N])/h,1);
                        return h;
                    }
                    else
                    {
                        // generic case
                        klo=1;
                        khi=2;
                        while( !(th>=theta[klo]&&th<=theta[khi]) )
                        {
                            ++klo;
                            ++khi;
                        }
                        const T tlo = theta[klo];
                        const T thi = theta[khi];
                        const T h   = thi-tlo;
                        A =  clamp<T>(0,(thi-th)/h,1);
                        B =  clamp<T>(0,(th-tlo)/h,1);
                        return h;
                    }
                }
            }

            static inline void emit(ios::ostream &fp,
                                    const vertex &p)
            {
                fp("%.15g %.15g\n", double(p.x), double(p.y));
            }


            static inline
            T max_norm_of(const vertices &p) noexcept
            {
                T res = hypothenuse(p[1]);
                for(size_t i=p.size();i>1;--i)
                {
                    const T tmp = hypothenuse(p[i]);
                    if(tmp>res) res = tmp;
                }
                return res;
            }


            static inline
            T max_abs_of(const rvector &p) noexcept
            {
                T res = std::abs(p[1]);
                for(size_t i=p.size();i>1;--i)
                {
                    const T tmp = std::abs(p[i]);
                    if(tmp>res) res = tmp;
                }
                return res;
            }



            inline
            void make_speed(const double dth, const size_t i, const size_t ip1) noexcept
            {
                static const T six(6);
                const vertex dp  = (*this)[ip1] - (*this)[i];
                coerce(speed)[i] = dp/dth - dth*(accel[i]+accel[i]+accel[ip1])/six;
            }

            inline
            double approx_curv(const size_t i) noexcept
            {
                const vertex &s   = speed[i];
                const T       sn  = hypothenuse(s);
                coerce(tvec[i])   = s/sn;
                coerce(nvec[i].x) =  tvec[i].y;
                coerce(nvec[i].y) = -tvec[i].x;
                const vertex  &a  =  accel[i];
                return (s.x * a.y - s.y * a.x)/(sn*sn*sn);
            }

            inline
            void local_frame()
            {
                for(size_t i=size();i>0;--i)
                    coerce(curv[i])   = approx_curv(i) / curv_scale;
                coerce(cmax) = max_abs_of(curv);
            }

            void make_spline()
            {
                //--------------------------------------------------------------
                // build spline matrix
                //--------------------------------------------------------------
                const size_t n = size();

                sm.ld(0);
                sm[1][1] = (theta[2] - theta_prev)/3; assert( sm[1][1] > 0);
                sm[1][2] = (theta[2] - theta[1]  )/6; assert( sm[1][2] > 0);
                sm[1][n] = (theta[1] - theta_prev)/6; assert( sm[1][n] > 0);

                for(size_t i=2;i<n;++i)
                {
                    sm[i][i-1] = ( theta[i]   - theta[i-1] )/6; assert( sm[i][i-1] > 0);
                    sm[i][i]   = ( theta[i+1] - theta[i-1] )/3; assert( sm[i][i]   > 0);
                    sm[i][i+1] = ( theta[i+1] - theta[i]   )/6; assert( sm[i][i+1] > 0);
                }

                sm[n][n]   = ( theta_next - theta[n-1] )/3; assert( sm[n][n] > 0);
                sm[n][n-1] = ( theta[n]   - theta[n-1] )/6; assert( sm[n][n-1] > 0);
                sm[n][1]   = ( theta_next - theta[n]   )/6; assert( sm[n][1] > 0);

                //std::cerr << "sm=" << sm << std::endl;
                //--------------------------------------------------------------
                // decompose matrix
                //--------------------------------------------------------------
                if(!lu.build(sm)) raise_singular();

                //--------------------------------------------------------------
                // compute accel per coordinate
                //--------------------------------------------------------------
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
                coerce(amax) = max_norm_of(accel);

                //--------------------------------------------------------------
                // compute speed
                //--------------------------------------------------------------
                for(size_t i=1;i<n;++i)
                    make_speed(theta[i+1]-theta[i],i,i+1);
                make_speed(theta_next-theta[n],n,1);

                coerce(smax) = max_norm_of(speed);

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

