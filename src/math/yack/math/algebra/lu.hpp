//! \file

#ifndef YACK_MATH_LU_INCLUDED
#define YACK_MATH_LU_INCLUDED 1

#include "yack/math/algebra/lu_.hpp"

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
            thin_array<scalar_type>              scal;        //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup to solve up to nmax*nmax systems
            explicit inline lu(const size_t nmax) :
            lu_(nmax,sizeof(T),make,done),
            scal( scal_<scalar_type>(),dims)
            {}

            //! cleanup
            virtual ~lu() throw() {}


            //__________________________________________________________________
            //
            // methods
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
                writable<size_t> &indx = *this;
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
                for (size_t j=1;j<=n;++j)
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
                T d = a[1][1];
                for(size_t i=a.rows;i>1;--i) d *= a[i][i];
                return dneg ? -d:d;
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
                assert(b.size()==a.rows);
                const size_t            n    = a.rows;
                const readable<size_t> &indx = *this;
                size_t                  ii   = 0;
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
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lu);
            static inline void make(void *addr)
            {
                new (addr) scalar_type();
            }
            
            static inline void done(void *addr) throw()
            {
                destruct( static_cast<scalar_type *>(addr) );
            }
        };
        
    }
}


#endif

