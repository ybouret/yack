//! \file

#ifndef YACK_MATH_LU_INCLUDED
#define YACK_MATH_LU_INCLUDED 1

#include "yack/math/algebra/lu_.hpp"
#include "yack/memory/operative.hpp"

namespace yack
{
    namespace math
    {
        
        //______________________________________________________________________
        //
        //
        //! LU decomposition and system solving
        //
        //______________________________________________________________________
        template <typename T> class lu : public lu_
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef typename scalar_for<T>::type scalar_type; //!< alias
            
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            //! setup to solve up to nmax*nmax systems
            explicit inline lu(const size_t nmax) :
            lu_(nmax,sizeof(scalar_type),sizeof(T)),
            indx(indx_(),dims),
            scal(scal_<scalar_type>(),dims),
            xrow(xrow_<T>(),dims),
            s_op(&scal[1],dims),
            t_op(&xrow[1],dims)
            {}
            
            //! cleanup
            virtual ~lu() throw() {}
            
            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________
            
            //__________________________________________________________________
            //
            //! build decomposition, matrix is altered
            //__________________________________________________________________
            inline bool build(matrix<T> &a)
            {
                //--------------------------------------------------------------
                //
                // sanity check and initialize
                //
                //--------------------------------------------------------------
                assert(a.is_square());
                assert(a.rows>0);
                assert(a.rows<=dims);
                const scalar_type one  = 1;
                const size_t      n    = a.rows;
                dneg = false;
                
                //--------------------------------------------------------------
                //
                // get pivot per row
                //
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                {
                    const readable<T> &r_i = a[i];
                    scalar_type        piv = abs_of(r_i[1]);
                    for(size_t j=n;j>1;--j)
                    {
                        const scalar_type tmp = abs_of(r_i[j]);
                        if(tmp>piv) piv=tmp;
                    }
                    if(piv<=0) return false; // a zero row
                    scal[i] = one/piv;
                }
                
                //--------------------------------------------------------------
                //
                // Crout's algorithm
                //
                //--------------------------------------------------------------
                for(size_t j=1;j<=n;++j)
                {
                    for(size_t i=1;i<j;++i)
                    {
                        writable<T> &a_i = a[i];
                        T sum=a_i[j];
                        for(size_t k=1;k<i;++k)
                            sum -= a_i[k]*a[k][j];
                        a_i[j]=sum;
                    }
                    size_t       imax = 0;
                    writable<T> &a_j  = a[j];
                    {
                        scalar_type vmax  = 0;
                        for (size_t i=j;i<=n;i++)
                        {
                            writable<T> &a_i = a[i];
                            T sum=a_i[j];
                            for (size_t k=1;k<j;++k)
                                sum -= a_i[k]*a[k][j];
                            a_i[j]=sum;
                            const scalar_type dum = scal[i] * abs_of(sum);
                            if ( dum >= vmax)
                            {
                                vmax=dum;
                                imax=i;
                            }
                        }
                    }
                    
                    if (j != imax)
                    {
                        a.swap_rows(j,imax);
                        dneg       = !dneg;
                        scal[imax] = scal[j];
                    }
                    
                    if( abs_of(a_j[j]) <= 0 )
                        return false;
                    
                    indx[j] = imax;
                    if(j!=n)
                    {
                        const T fac=one/(a_j[j]);
                        for(size_t i=n;i>j;--i)
                            a[i][j] *= fac;
                    }
                }
                
                return true;
            }
            
            //__________________________________________________________________
            //
            //! return determinant of a LU matrix
            //__________________________________________________________________
            inline T det(const matrix<T> &a) const
            {
                assert(a.is_square());
                assert(a.rows>0);
                assert(a.rows<=dims);
                T d = dneg ? -1:1;
                for(size_t i=a.rows;i>0;--i) d *= a[i][i];
                return d;
            }
            
            //__________________________________________________________________
            //
            //! in-place solve a*u=b
            //__________________________________________________________________
            inline void solve(const matrix<T> &a, writable<T> &b) const
            {
                assert(a.is_square());
                assert(a.rows>0);
                assert(a.rows<=dims);
                assert(b.size()>=a.rows);
                const size_t n  = a.rows;
                size_t       ii = 0;
                for(size_t i=1;i<=n;i++)
                {
                    size_t ip=indx[i];
                    T sum=b[ip];
                    b[ip]=b[i];
                    if (ii)
                    {
                        const readable<T> &a_i = a[i];
                        for(size_t j=ii;j<i;++j)
                            sum -= a_i[j]*b[j];
                    }
                    else
                    {
                        if( abs_of(sum) > 0) ii=i;
                    }
                    b[i] = sum;
                }
                
                for (size_t i=n;i>=1;i--)
                {
                    const readable<T> &a_i = a[i];
                    T sum=b[i];
                    for(size_t j=n;j>i;--j)
                        sum -= a_i[j]*b[j];
                    b[i]=sum/a_i[i];
                }
            }
            
            //__________________________________________________________________
            //
            //! compute inverse matrix from decomposition
            //__________________________________________________________________
            inline void inv(matrix<T> &q, const matrix<T> &a)
            {
                assert(a.is_square());
                assert(a.rows>0);
                assert(a.rows<=dims);
                assert(matrix_metrics::have_same_sizes(q,a));
                const size_t  n    = a.rows;
                thin_array<T> u(xrow_<T>(),n);
                for(size_t j=n;j>0;--j)
                {
                    for(size_t k=n;k>0;--k) u[k] = 0;
                    u[j] = 1;
                    solve(a,u);
                    for(size_t k=n;k>0;--k) q[k][j] = u[k];
                }
            }
            
            //__________________________________________________________________
            //
            //! compute adjoint matrix
            //__________________________________________________________________
            template <typename U>
            inline void adj(matrix<T> &q, const matrix<U> &a)
            {
                assert(a.is_square());
                assert(a.rows<=dims+1);
                assert(matrix_metrics::have_same_sizes(q,a));
                const size_t  n    = a.rows;
                switch(n)
                {
                    case 0: return;
                    case 1: q[1][1] = 1; return;
                    default:
                        break;
                }
                matrix<T> m(n-1,n-1);
                for(size_t j=n;j>0;--j)
                {
                    matrix_row<T> &q_j = q[j];
                    for(size_t i=n;i>0;--i)
                    {
                        a.compute_minor(m,i,j);
                        if(build(m))
                            q_j[i] = (0==((i+j)&1)) ? det(m) : -det(m);
                        else
                            q_j[i] = 0;
                    }
                }
            }
            
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lu);
            thin_array<size_t>                       indx; //!< indices
            thin_array<scalar_type>                  scal; //!< scaling
            thin_array<T>                            xrow; //!< extra row/col
            const memory::operative_of<scalar_type>  s_op; //!< memory I/O for scalar
            const memory::operative_of<T>            t_op; //!< memory I/O for objects            
        };
        
    }
}


#endif

