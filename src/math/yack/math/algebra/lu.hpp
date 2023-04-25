//! \file

#ifndef YACK_MATH_LU_INCLUDED
#define YACK_MATH_LU_INCLUDED 1

#include "yack/math/algebra/lu-kernel.hpp"
#include "yack/container/matrix.hpp"
#include "yack/ipso/add.hpp"
#include "yack/ipso/mul.hpp"
#include "yack/memory/operative.hpp"

namespace yack
{
    namespace math
    {

        template <typename T>
        class LU : public LU_Kernel
        {
        public:
            YACK_DECL_ARGS(T,type); //!< aliases
            typedef ipso::inside<T>                        inside;
            typedef typename inside::type                  inside_type;
            typedef typename scalar_for<inside_type>::type scalar_type;

            inline virtual ~LU() noexcept {}

            inline explicit LU(const size_t n) :
            LU_Kernel(n,sizeof(scalar_type),sizeof(inside_type)),
            dcmp(nmax,nmax),
            xadd(nmax),
            xmul(nmax),
            indx(indx_,nmax),
            smem(scal_,nmax),
            scal(static_cast<scalar_type *>(scal_),nmax),
            xmem(xtra_,nmax),
            result(static_cast<inside_type *>(xtra_),nmax),
            scalar0(0),
            scalar1(1),
            inside0(0),
            inside1(1)
            {
                assert( xadd.capacity() >= nmax );
                assert( xmul.capacity() >= nmax );

            }

            bool build(const matrix<T> &a)
            {
                assert(a.is_square());
                assert(a.rows<=nmax);
                if(!initialize(a)) return false;
                return decomposed(a.rows);
            }

            void solve(const size_t n)
            {
                assert(n<=nmax);
                writable<inside_type> &b = result;
                size_t ii = 0;
                for(size_t i=1;i<=n;i++) {
                    const readable<inside_type> &d_i = dcmp[i];
                    const size_t ip  = indx[i]; assert(ip>0); assert(ip<=n);
                    inside_type  sum = b[ip];
                    b[ip]=b[i];
                    if(ii)
                    {
                        for (size_t j=ii;j<=i-1;j++) sum -= d_i[j]*b[j];
                    }
                    else
                    {
                        if( abs_of(sum) > scalar0) ii=i;
                    }
                    b[i]=sum;
                }

                for(size_t i=n;i>0;--i) {
                    const readable<inside_type> &d_i = dcmp[i];
                    xadd.free();
                    xadd << b[i];
                    for(size_t j=n;j>i;--j) xadd << -d_i[j]*b[j];
                    b[i] = xadd.reduce()/d_i[i];
                }
            }

            template <typename ARRAY>
            void solve(ARRAY &b)
            {
                const size_t n = b.size(); assert(n<=nmax);
                for(size_t i=n;i>0;--i) result[i] = inside::send(b[i]);
                solve(n);
            }

            template <typename ARRAY>
            void load(ARRAY &u) const
            {
                const size_t n = u.size(); assert(n<=nmax);
                for(size_t i=n;i>0;--i) u[i] = inside::recv(result[i]);
            }

            inside_type det(const size_t n)
            {
                xmul.free();
                for(size_t i=n;i>0;--i) xmul.insert(dcmp[i][i]);
                return dneg ? -xmul.reduce() : xmul.reduce();
            }


            matrix<inside_type>                     dcmp;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(LU);
            ipso::add<inside_type>                  xadd;
            ipso::mul<type>                         xmul;
            thin_array<size_t>                      indx;
            const memory::operative_of<scalar_type> smem;
            thin_array<scalar_type>                 scal;
            const memory::operative_of<inside_type> xmem;
        public:
            thin_array<inside_type>                 result;
            const scalar_type                       scalar0;
            const scalar_type                       scalar1;
            const inside_type                       inside0;
            const inside_type                       inside1;

        private:
            inline bool initialize(const matrix<T> &a)
            {
                const size_t n = a.rows;
                coerce(dneg) = false;
                for(size_t i=n;i>0;--i)
                {
                    scalar_type           piv = scalar0;
                    const readable<T>     &a_i = a[i];
                    writable<inside_type> &d_i = dcmp[i];

                    for(size_t j=n;j>0;--j)
                    {
                        const scalar_type tmp = abs_of<inside_type>( d_i[j] = inside::send(a_i[j]) );
                        if(tmp>piv) piv=tmp;
                    }
                    if(piv<=0) return false;
                    scal[i] = scalar1/piv;
                }
                return true;
            }

            inline bool decomposed(const size_t n)
            {
                for(size_t j=1;j<=n;++j)
                {
                    for(size_t i=1;i<j;i++) {
                        xadd.free();
                        xadd << dcmp[i][j];
                        for(size_t k=1;k<i;k++)
                        {
                            xadd << -dcmp[i][k] * dcmp[k][j];
                        }
                        dcmp[i][j] = xadd.reduce();
                    }

                    scalar_type piv = scalar0;
                    size_t      imx = j;
                    for (size_t i=j;i<=n;i++)
                    {
                        xadd.free();
                        xadd << dcmp[i][j];
                        for (size_t k=1;k<j;k++)
                        {
                            xadd << -dcmp[i][k]*dcmp[k][j];
                        }
                        const scalar_type tmp = scal[i] * abs_of(dcmp[i][j]=xadd.reduce());
                        if(tmp>piv)
                        {
                            piv = tmp;
                            imx = i;
                        }
                    }

                    if(j!=imx)
                    {
                        dcmp.swap_rows(j,imx);
                        scal[j] = scal[imx];
                        coerce(dneg) = !dneg;
                    }

                    indx[j] = imx;
                    const inside_type &den = dcmp[j][j];
                    if(abs_of(den) <= scalar0 ) return false;

                    if (j != n) {
                        for(size_t i=n;i>j;--i) dcmp[i][j] /= den;
                    }
                }
                return true;
            }


        };

    }


}

#endif

