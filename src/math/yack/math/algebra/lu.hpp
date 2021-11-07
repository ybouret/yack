//! \file

#ifndef YACK_MATH_LU_INCLUDED
#define YACK_MATH_LU_INCLUDED 1

#include "yack/type/scalar.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/math/api.hpp"

namespace yack
{
    namespace math
    {
        class lu_ : public writable<size_t>, public dynamic
        {
        public:
            typedef void (*proc)(void*);
            virtual ~lu_() throw();
            
            const size_t dims;
            
            virtual size_t         size() const throw();
            virtual size_t &       operator[](const size_t) throw();
            virtual const size_t & operator[](const size_t) const throw();
            virtual size_t         granted() const throw();
            
        protected:
            explicit lu_(const size_t nmax,
                         const size_t itsz,
                         proc         make,
                         proc         done);
            
            void *get_scal() throw();
            
            bool   dneg;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(lu_);
            size_t      *upos; //!< [1..size]
            uint8_t     *data; //!<
            const size_t step; //!<
            proc         kill; //!<
            void        *wksp;
            size_t       wlen;
            void         clear(size_t) throw();
        };
        
        template <typename T> class lu :
        public lu_
        {
        public:
            typedef typename scalar_for<T>::type scalar_type;
            thin_array<scalar_type>              scal;
            
            explicit inline lu(const size_t nmax) :
            lu_(nmax,sizeof(T),make,done),
            scal( static_cast<scalar_type*>(get_scal()),dims)
            {
            }
            
            virtual ~lu() throw()
            {
            }
            
            inline bool build(matrix<T> &a) throw()
            {
                //--------------------------------------------------------------
                //
                // sanity check
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
                // Crout's
                //
                //--------------------------------------------------------------
                for (size_t j=1;j<=n;++j)
                {
                    for(size_t i=1;i<j;++i)
                    {
                        T sum=a[i][j];
                        for(size_t k=1;k<i;++k) sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
                    }
                    scalar_type vmax  = 0;
                    size_t      imax = 0;
                    for (size_t i=j;i<=n;i++)
                    {
                        T sum=a[i][j];
                        for (size_t k=1;k<j;++k)
                            sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;
                        const scalar_type dum = scal[i] * abs_of(sum);
                        if ( dum >= vmax)
                        {
                            vmax=dum;
                            imax=i;
                        }
                    }
                    if (j != imax)
                    {
                        a.swap_rows(j,imax);
                        dneg       = !dneg;
                        scal[imax] = scal[j];
                    }
                    if( abs_of(a[j][j]) <= 0 )
                        return false;
                    indx[j] = imax;
                    if(j!=n)
                    {
                        const T fac=one/(a[j][j]);
                        for(size_t i=j+1;i<=n;++i) a[i][j] *= fac;
                    }
                }
                
                return true;
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

