//! \file

#ifndef YACK_CROUT_INCLUDED
#define YACK_CROUT_INCLUDED 1

#include "yack/math/api.hpp"
#include "yack/math/algebra/crout_.hpp"
#include "yack/container/matrix.hpp"
#include "yack/memory/operative.hpp"
#include "yack/math/adder.hpp"
#include "yack/math/multiplier.hpp"

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
            // types and definitions
            //__________________________________________________________________
            YACK_DECL_ARGS_(T,type); //!< aliaes
            typedef typename scalar_for<mutable_type>::type scalar_type; //!< alias
            typedef const scalar_type                       const_scalar_type; //!< alias
            
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            inline virtual ~crout() throw() {}

            //! setup to solve up to given dimension
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
            // methods
            //__________________________________________________________________

            void pass1(matrix<T> &a,   const size_t j)
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

            void pass1(matrix<T> &a, const size_t j, adder<T> &xadd)
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

            //! try to build the LU decomposition a a square matrix
            inline bool build(matrix<T> &a, adder<T> *xadd= NULL)
            {
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
                    if(xadd)
                        pass1(a,j,*xadd);
                    else
                        pass1(a,j);

                    //----------------------------------------------------------
                    // pass 2
                    //----------------------------------------------------------
                    size_t      imax=j;
                    scalar_type smax=0;
                    for(size_t i=j;i<=n;++i)
                    {
                        writable<type> &a_i = a[i];
                        type sum=a_i[j];
                        for (size_t k=1;k<j;k++)
                            sum -= a_i[k]*a[k][j];
                        a_i[j]=sum;

                        const scalar_type temp = scal[i] * abs_of<type>(sum);
                        if(temp >= smax)
                        {
                            smax=temp;
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
                        for(size_t i=j+1;i<=n;++i)
                            a[i][j] *= fac;
                    }
                }

                return true;
            }

            //! in place solve with a=LU
            inline void solve(const matrix<T> &a, writable<T> &b)
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows <=nmax);
                assert(a.cols==b.size());

                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                const size_t            n = a.rows;
                thin_array<size_t>      indx(indx_,n);
                thin_array<scalar_type> scal(static_cast<scalar_type*>(scal_),n);

                //______________________________________________________________
                //
                // pass 1
                //______________________________________________________________
                size_t ii=0;
                for(size_t i=1;i<=n;++i)
                {
                    const readable<T> &a_i = a[i];
                    size_t ip  = indx[i];
                    type   sum = b[ip];
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

                //______________________________________________________________
                //
                // pass 2
                //______________________________________________________________
                for (size_t i=n;i>0;--i) {
                    const readable<T> &a_i = a[i];
                    type sum=b[i];
                    for (size_t j=i+1;j<=n;++j)
                        sum -= a_i[j]*b[j];
                    b[i]=sum/a_i[i];
                }
            }

            //! in place multiple solve with a=LU
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

            //! determinant of a LU matrix
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
            
            //! determinant of a LU matrix with precise multiplication
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

            //! determinant with optional multiplier
            inline type det(const matrix<T> &a, multiplier<T> *xmul) const
            {
                return xmul ? determinant(a,*xmul) : determinant(a);
            }


            //! compute inverse from LU matrix
            inline void inverse(const matrix<T> &a, matrix<T> &I)
            {
                //______________________________________________________________
                //
                // sanity check
                //______________________________________________________________
                assert(a.is_square());
                assert(a.rows <=nmax);
                assert(a.rows>0);
                assert(matrix_metrics::have_same_sizes(a,I));

                //______________________________________________________________
                //
                // build identity matrix
                //______________________________________________________________
                const size_t     n = a.rows;
                { const type _0(0); I.ld(_0); }
                for(size_t i=n;i>0;--i)
                {
                    I[i][i] = t_one;
                }

                //______________________________________________________________
                //
                // solve identity matrix
                //______________________________________________________________
                solve(a,I);
            }
            
            //! compute adjoint matrix : source * target = det(target) * Id
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
            





        private:
            YACK_DISABLE_COPY_AND_ASSIGN(crout);
            const_scalar_type                       s_one;
            const_type                              t_one;
            const memory::operative_of<scalar_type> scalM;
            const memory::operative_of<type>        xtraM;

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

        };


    }

}

#endif
