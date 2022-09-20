//! \file

#ifndef YACK_CROUT_INCLUDED
#define YACK_CROUT_INCLUDED 1

#include "yack/math/api.hpp"
#include "yack/math/algebra/crout_.hpp"
#include "yack/container/matrix.hpp"
#include "yack/memory/operative.hpp"
#include "yack/math/adder.hpp"
#include "yack/math/multiplier.hpp"

//#define YACK_CROUT_SENTRY

#if defined(YACK_CROUT_SENTRY)
#include "yack/memory/sentry.hpp"
#endif

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! Crout's algorithm fo LU decomposition
        //
        //______________________________________________________________________
        template <typename T>
        class crout : public crout_
        {
        public:
            //__________________________________________________________________
            //
            //
            // types and definitions
            //
            //__________________________________________________________________
            YACK_DECL_ARGS_(T,type);                                           //!< aliases
            typedef typename scalar_for<mutable_type>::type scalar_type;       //!< alias
            typedef const scalar_type                       const_scalar_type; //!< alias
            
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            
            //__________________________________________________________________
            //
            //! cleanup
            //__________________________________________________________________
            inline virtual ~crout() throw() {}

            //__________________________________________________________________
            //
            //! setup to solve up to given dimension
            //__________________________________________________________________
            inline explicit crout(const size_t dimension) :
            crout_(dimension,sizeof(type),sizeof(scalar_type)),
            s_one(1),
            t_one(1),
            scalM(scal_,nmax),
            xtraM(xtra_,nmax)
            {
            }

            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! try to build the LU decomposition of a square matrix
            //__________________________________________________________________
            inline bool build(matrix<T> &a)
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                if(!initialize(a)) return false;
                const size_t            n = a.rows;
                thin_array<size_t>      indx(indx_,n);
                thin_array<scalar_type> scal(static_cast<scalar_type*>(scal_),n);

#if defined(YACK_CROUT_SENTRY)
                YACK_MEM_SENTRY(xtra_,sizeof(type)*nmax);
#endif

                //______________________________________________________________
                //
                // loop over columns
                //______________________________________________________________
                for(size_t j=1;j<=n;++j)
                {
                    //----------------------------------------------------------
                    // pass 1
                    //----------------------------------------------------------
                    pass1(a,j);

                    //----------------------------------------------------------
                    // pass 2
                    //----------------------------------------------------------
                    size_t      imax=j;
                    scalar_type smax=0;
                    for(size_t i=j;i<=n;++i)
                    {
                        const scalar_type f = pass2(a,i,j); assert(f>=0);
                        const scalar_type t = scal[i] * f;  assert(t>=0);
                        if(t >= smax)
                        {
                            smax=t;
                            imax=i;
                        }
                    }

                    //----------------------------------------------------------
                    // check if need to exchange
                    //----------------------------------------------------------
                    if(imax!=j)
                    {
                        a.swap_rows(imax,j);
                        coerce(dneg) = !dneg;
                        scal[imax]   = scal[j];
                    }

                    //----------------------------------------------------------
                    // fill processing index
                    //----------------------------------------------------------
                    indx[j] = imax;

                    //----------------------------------------------------------
                    // check not singular up to precision
                    //----------------------------------------------------------
                    const_type &a_jj = a[j][j];
                    if(abs_of<type>(a_jj) <= 0) return false;

                    //----------------------------------------------------------
                    // finalize
                    //----------------------------------------------------------
                    if (j != n)
                    {
                        const_type fac=t_one/a_jj;
                        for(size_t i=n;i>j;--i)
                            a[i][j] *= fac;
                    }
                }

                return true;
            }

            //__________________________________________________________________
            //
            //! try to build the LU decomposition a square matrix, with effort
            //__________________________________________________________________
            inline bool build(matrix<T> &a, adder<T> &xadd)
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                if(!initialize(a)) return false;
                const size_t            n = a.rows;
                thin_array<size_t>      indx(indx_,n);
                thin_array<scalar_type> scal(static_cast<scalar_type*>(scal_),n);

                //______________________________________________________________
                //
                // loop over columns
                //______________________________________________________________
                for(size_t j=1;j<=n;++j)
                {
                    //----------------------------------------------------------
                    // pass 1
                    //----------------------------------------------------------
                    pass1(a,j,xadd);


                    //----------------------------------------------------------
                    // pass 2
                    //----------------------------------------------------------
                    size_t      imax=j;
                    scalar_type smax=0;
                    for(size_t i=j;i<=n;++i)
                    {
                        const scalar_type f = pass2(a,i,j,xadd);
                        const scalar_type t = scal[i] * f;
                        if(t >= smax)
                        {
                            smax=t;
                            imax=i;
                        }
                    }

                    //----------------------------------------------------------
                    // check if need to exchange
                    //----------------------------------------------------------
                    if(imax!=j)
                    {
                        a.swap_rows(imax,j);
                        coerce(dneg) = !dneg;
                        scal[imax]   = scal[j];
                    }

                    //----------------------------------------------------------
                    // fill processing index
                    //----------------------------------------------------------
                    indx[j] = imax;

                    //----------------------------------------------------------
                    // check not singular up to precision
                    //----------------------------------------------------------
                    const_type &a_jj = a[j][j];
                    if(abs_of<type>(a_jj) <= 0) return false;

                    //----------------------------------------------------------
                    // finalize
                    //----------------------------------------------------------
                    if (j != n)
                    {
                        const_type fac=t_one/a_jj;
                        for(size_t i=n;i>j;--i)
                            a[i][j] *= fac;
                    }
                }

                return true;
            }

            //__________________________________________________________________
            //
            //! in place solve with decomposed a
            //__________________________________________________________________
            inline void solve(const matrix<T> &a, writable<T> &b) const
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows<=nmax);
                assert(a.cols==b.size());

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const size_t n = a.rows;


#if defined(YACK_CROUT_SENTRY)
                YACK_MEM_SENTRY(indx_,sizeof(size_t)*nmax);
                YACK_MEM_SENTRY(scal_,sizeof(scalar_type)*nmax);
#endif
                //______________________________________________________________
                //
                // forward
                //______________________________________________________________
                {
                    const thin_array<size_t> indx(indx_,n);
                    size_t                   ii=0;
                    for(size_t i=1;i<=n;++i)
                    {
                        const readable<T> &a_i  = a[i];
                        const size_t        ip  = indx[i];
                        type                sum = b[ip];
                        b[ip]=b[i];
                        if (ii)
                        {
                            for(size_t j=ii;j<i;++j)
                            {
                                sum -= a_i[j]*b[j];
                            }
                        }
                        else
                        {
                            if( abs_of<type>(sum)>0 )
                                ii=i;
                        }
                        b[i]=sum;
                    }
                }

                //______________________________________________________________
                //
                // reverse
                //______________________________________________________________
                for (size_t i=n;i>0;--i) {
                    const readable<T> &a_i = a[i];
                    type sum=b[i];
                    for (size_t j=i+1;j<=n;++j)
                        sum -= a_i[j]*b[j];
                    b[i]=sum/a_i[i];
                }
            }

            //__________________________________________________________________
            //
            //! in place solve with decomposed matrix and effort
            //__________________________________________________________________
            inline void solve(const matrix<T> &a, writable<T> &b, adder<T> &xadd) const
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows<=nmax);
                assert(a.cols==b.size());

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const size_t n = a.rows;

#if defined(YACK_CROUT_SENTRY)
                YACK_MEM_SENTRY(indx_,sizeof(size_t)*nmax);
                YACK_MEM_SENTRY(scal_,sizeof(scalar_type)*nmax);
#endif
                //______________________________________________________________
                //
                // forward
                //______________________________________________________________
                {
                    const thin_array<size_t> indx(indx_,n);
                    size_t                   ii=0;
                    for(size_t i=1;i<=n;++i)
                    {
                        const readable<T> &a_i = a[i];
                        size_t ip  = indx[i];
                        type   sum = b[ip];
                        b[ip]=b[i];
                        if(ii)
                        {
                            xadd.ldz();
                            xadd.push(sum);
                            for(size_t j=ii;j<i;++j)
                            {
                                xadd.push(-a_i[j]*b[j]);
                            }
                            sum = xadd.get();
                        }
                        else
                        {
                            if( abs_of<type>(sum)>0 )
                                ii=i;
                        }
                        b[i]=sum;
                    }
                }

                //______________________________________________________________
                //
                // reverse
                //______________________________________________________________
                for (size_t i=n;i>0;--i) {
                    const readable<T> &a_i = a[i];
                    xadd.ldz();
                    xadd.push(b[i]);
                    for (size_t j=n;j>i;--j)
                        xadd.push( -a_i[j]*b[j] );
                    b[i]=xadd.get()/a_i[i];
                }
            }



            //__________________________________________________________________
            //
            //! in place multiple solve with decomposed a
            //__________________________________________________________________
            inline void solve(const matrix<T> &a, matrix<T> &b)
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows <=nmax);
                assert(b.rows==a.rows);

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const size_t     n = a.rows;
                thin_array<type> u(static_cast<type*>(xtra_),n);

                //______________________________________________________________
                //
                // solve columns
                //______________________________________________________________
                for(size_t j=b.cols;j>0;--j)
                {
                    for(size_t i=n;i>0;--i) u[i] = b[i][j];
                    solve(a,u);
                    for(size_t i=n;i>0;--i) b[i][j] = u[i];
                }
            }

            //__________________________________________________________________
            //
            //! in place multiple solve with decomposed matrix and effort
            //__________________________________________________________________
            inline void solve(const matrix<T> &a, matrix<T> &b, adder<T> &xadd)
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows <=nmax);
                assert(b.rows==a.rows);

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const size_t     n = a.rows;
                thin_array<type> u(static_cast<type*>(xtra_),n);

                //______________________________________________________________
                //
                // solve columns
                //______________________________________________________________
                for(size_t j=b.cols;j>0;--j)
                {
                    for(size_t i=n;i>0;--i) u[i] = b[i][j];
                    solve(a,u,xadd);
                    for(size_t i=n;i>0;--i) b[i][j] = u[i];
                }
            }


            //__________________________________________________________________
            //
            //! determinant of a decomposed matrix
            //__________________________________________________________________
            inline type determinant(const matrix<T> &a) const
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows<=nmax);
                assert(a.rows>0);

                //______________________________________________________________
                //
                // diagonal product
                //______________________________________________________________
                type res = a[1][1];
                for(size_t i=a.rows;i>1;--i) res *= a[i][i];
                return dneg ? -res : res;
            }

            //__________________________________________________________________
            //
            //! determinant of a decomposed matrix with effort (integral types)
            //__________________________________________________________________
            inline type determinant(const matrix<T> &a, multiplier<T> &xmul) const
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows <=nmax);
                assert(a.rows>0);

                //______________________________________________________________
                //
                // diagonal product
                //______________________________________________________________
                xmul.set1();
                for(size_t i=a.rows;i>0;--i) xmul *= a[i][i];
                return dneg ? -xmul.query() : xmul.query();
            }


            //__________________________________________________________________
            //
            //! compute inverse from a decomposed matrix
            //__________________________________________________________________
            inline void inverse(const matrix<T> &a, matrix<T> &I)
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows<=nmax);
                assert(a.rows>0);
                assert(matrix_metrics::have_same_sizes(a,I));

                //______________________________________________________________
                //
                // build identity matrix
                //______________________________________________________________
                Id(I);

                //______________________________________________________________
                //
                // solve identity matrix
                //______________________________________________________________
                solve(a,I);
            }


            //__________________________________________________________________
            //
            //! compute inverse from a decomposed matrix and effort
            //__________________________________________________________________
            inline void inverse(const matrix<T> &a, matrix<T> &I, adder<T> &xadd)
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows<=nmax);
                assert(a.rows>0);
                assert(matrix_metrics::have_same_sizes(a,I));

                //______________________________________________________________
                //
                // build identity matrix
                //______________________________________________________________
                Id(I);

                //______________________________________________________________
                //
                // solve identity matrix
                //______________________________________________________________
                solve(a,I,xadd);
            }


            //__________________________________________________________________
            //
            //! compute adjoint matrix : source * target = det(target) * Id
            //__________________________________________________________________
            template <typename U>
            inline void adjoint(matrix<T> &target, const matrix<U> &source)
            {
                assert(target.is_square());
                assert(target.rows <=nmax);
                assert(matrix_metrics::have_same_sizes(target,source));
                
                const size_t n = target.rows;
                switch(n)
                {
                    case 0:
                        return;
                    case 1:
                        target[1][1] = 1;
                        return;
                    default:
                        break;
                }
                matrix<T> m(n-1,n-1);
                for(size_t i=n;i>0;--i)
                {
                    for(size_t j=n;j>0;--j)
                    {
                        source.compute_minor(m,j,i);
                        if(build(m))
                        {
                            const bool minus = 0 != ( (i+j) & 1);
                            target[i][j] =  minus ?  -determinant(m) : determinant(m);
                        }
                        else
                        {
                            target[i][j] = 0;
                        }
                    }
                }
            }
            
            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const_scalar_type                       s_one; //!< scalar(1)
            const_type                              t_one; //!< type(1)

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(crout);

            const memory::operative_of<scalar_type> scalM;
            const memory::operative_of<type>        xtraM;

            // =================================================================
            //! building Id
            // =================================================================
            inline void Id(matrix<T> &I) const
            {
                assert(I.is_square());
                { const type _0(0); I.ld(_0); }
                for(size_t i=I.rows;i>0;--i)
                {
                    I[i][i] = t_one;
                }
            }

            // =================================================================
            //! initializing
            // =================================================================
            inline bool initialize(matrix<T> &a)
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows <=nmax);

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const size_t            n = a.rows;
                thin_array<size_t>      indx(indx_,n);
                thin_array<scalar_type> scal(static_cast<scalar_type*>(scal_),n);
                coerce(dneg) = false;

                //______________________________________________________________
                //
                // check pivot per row
                //______________________________________________________________
                for(size_t i=n;i>0;--i)
                {
                    readable<type> &a_i = a[i];
                    scalar_type     piv = abs_of<type>(a_i[1]);
                    for(size_t j=n;j>1;--j)
                    {
                        const scalar_type tmp = abs_of<type>(a_i[j]);
                        if(tmp>piv)
                            piv=tmp;
                    }
                    if(piv<=0) return false; // row is zeroed...
                    scal[i] = s_one/piv;
                }

                //______________________________________________________________
                //
                // done
                //______________________________________________________________
                return true;
            }

            // =================================================================
            // pass1 algorithms
            // =================================================================

            inline void pass1(matrix<T> &a, const size_t j)
            {
                for(size_t i=1;i<j;i++)
                {
                    writable<type> &a_i = a[i];
                    type sum=a_i[j];
                    for (size_t k=1;k<i;k++)
                        sum -= a_i[k]*a[k][j];
                    a_i[j]=sum;
                }
            }

            inline void pass1(matrix<T> &a, const size_t j, adder<T> &xadd)
            {
                for(size_t i=1;i<j;i++)
                {
                    writable<type> &a_i = a[i];
                    xadd.ldz();
                    xadd.push(a_i[j]);
                    for (size_t k=1;k<i;k++)
                        xadd.push( -a_i[k]*a[k][j] );
                    a_i[j]=xadd.get();
                }
            }

            // =================================================================
            // pass2 algorithms
            // =================================================================

            inline scalar_type pass2(matrix<T> &a, const size_t i, const size_t j)
            {
                writable<type> &a_i = a[i];
                type sum=a_i[j];
                for (size_t k=1;k<j;k++)
                    sum -= a_i[k]*a[k][j];
                return abs_of(a_i[j]=sum);
            }

            inline scalar_type pass2(matrix<T> &a, const size_t i, const size_t j, adder<T> &xadd)
            {
                writable<type> &a_i = a[i];
                xadd.ldz();
                xadd.push(a_i[j]);
                for (size_t k=1;k<j;k++)
                    xadd.push( -a_i[k]*a[k][j] );
                return abs_of(a_i[j]=xadd.get());
            }

        };


    }

}

#endif
