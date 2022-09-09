//! \file

#ifndef YACK_CROUT_INCLUDED
#define YACK_CROUT_INCLUDED 1

#include "yack/math/api.hpp"
#include "yack/math/algebra/crout_.hpp"
#include "yack/container/matrix.hpp"
#include "yack/memory/operative.hpp"

namespace yack
{
    namespace math
    {

        template <typename T>
        class crout : public crout_
        {
        public:

            YACK_DECL_ARGS_(T,type);
            typedef typename scalar_for<mutable_type>::type scalar_type;
            typedef const scalar_type                       const_scalar_type;
            

            inline virtual ~crout() throw() {}

            inline explicit crout(const size_t dimension) :
            crout_(dimension,sizeof(type),sizeof(scalar_type)),
            s_one(1),
            t_one(1),
            scalM(scal_,nmax),
            xtraM(xtra_,nmax)
            {
            }


            bool initialize(matrix<T> &a)
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

                std::cerr << "scal=" << scal << std::endl;
                return true;
            }


            inline bool build(matrix<T> &a)
            {
                if(!initialize(a)) return false;
                const size_t            n = a.rows;
                thin_array<size_t>      indx(indx_,n);
                thin_array<scalar_type> scal(static_cast<scalar_type*>(scal_),n);

                // loop over column
                for(size_t j=1;j<=n;++j)
                {

                    // pass 1
                    for(size_t i=1;i<j;i++)
                    {
                        writable<type> &a_i = a[i];
                        type sum=a_i[j];
                        for (size_t k=1;k<i;k++)
                            sum -= a_i[k]*a[k][j];
                        a_i[j]=sum;
                    }

                    // pass 2
                    size_t      imax=j;
                    scalar_type piv=0;
                    for(size_t i=j;i<=n;++i)
                    {
                        type sum=a[i][j];
                        for (size_t k=1;k<j;k++)
                            sum -= a[i][k]*a[k][j];
                        a[i][j]=sum;

                        const scalar_type tmp = scal[i] * abs_of<type>(sum);
                        if( tmp >= piv)
                        {
                            piv=tmp;
                            imax=i;
                        }
                    }

                    // check if need to exchange
                    if(imax!=j)
                    {
                        a.swap_rows(imax,j);
                        coerce(dneg) = !dneg;
                        scal[imax]   = scal[j];
                    }

                    // fill processing index
                    indx[j] = imax;

                    // check not singular up to precision
                    const_type &a_jj = a[j][j];
                    if(abs_of<type>(a[j][j]) <= 0) return false;

                    // finalize
                    if (j != n)
                    {
                        const_type fac=t_one/a_jj;
                        for(size_t i=j+1;i<=n;++i)
                            a[i][j] *= fac;

                    }
                }

                return true;
            }

            inline void solve(const matrix<T> &a, writable<T> &b)
            {

                assert(a.cols==b.size());

                const size_t            n = a.rows;
                thin_array<size_t>      indx(indx_,n);
                thin_array<scalar_type> scal(static_cast<scalar_type*>(scal_),n);

                size_t ii=0;
                for(size_t i=1;i<=n;++i)
                {
                    size_t ip=indx[i];
                    type sum=b[ip];
                    b[ip]=b[i];
                    if (ii)
                    {
                        for (size_t j=ii;j<=i-1;++j)
                        {
                            sum -= a[i][j]*b[j];
                        }
                    }
                    else
                    {
                        if( abs_of<type>(sum)>0 )
                            ii=i;
                    }
                    b[i]=sum;
                }

                for (size_t i=n;i>0;--i) {
                    type sum=b[i];
                    for (size_t j=i+1;j<=n;++j)
                        sum -= a[i][j]*b[j];
                    b[i]=sum/a[i][i];
                }
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(crout);
            const_scalar_type                       s_one;
            const_type                              t_one;
            const memory::operative_of<scalar_type> scalM;
            const memory::operative_of<type>        xtraM;


        };


    }

}

#endif
