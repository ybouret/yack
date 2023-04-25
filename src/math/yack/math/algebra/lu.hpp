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
        //______________________________________________________________________
        //
        //
        //! LU decomposition and solving
        //
        //______________________________________________________________________
        template <typename T>
        class LU : public matrix< typename ipso::inside<T>::type >, public LU_Kernel
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                                     //!< aliases
            typedef ipso::inside<T>                        inside;      //!< alias
            typedef typename inside::type                  inside_type; //!< internal computation
            typedef typename scalar_for<inside_type>::type scalar_type; //!< internal computation scalar
            typedef matrix<inside_type>                    matrix_type; //!< base class
            typedef ipso::add<type>                        xadd_type;   //!< alias
            typedef ipso::mul<type>                        xmul_type;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline virtual ~LU() noexcept {} //!< cleanup

            //! setup to solve up to [n x n] systems
            inline explicit LU(const size_t n) :
            matrix_type(n,n),
            LU_Kernel(n,sizeof(scalar_type),sizeof(inside_type)),
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

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! decompose matrix
            template <typename U>
            inline bool build(const matrix<U> &a)
            {
                assert(a.is_square());
                assert(a.rows<=nmax);
                if(!initialize(a)) return false;
                return decomposed(a.rows);
            }

            //! assume result[1..n] is ready
            inline void apply(const size_t n)
            {
                assert(n<=nmax);
                const matrix_type     &lu  = *this;
                writable<inside_type> &rhs = result;
                {
                    size_t ii = 0;
                    for(size_t i=1;i<=n;i++) {
                        const readable<inside_type> &lu_i = lu[i];
                        xadd.free();
                        const size_t ip  = indx[i]; assert(ip>0); assert(ip<=n);
                        xadd.grow(rhs[ip]);
                        rhs[ip]=rhs[i];
                        if(ii)
                        {
                            for (size_t j=ii;j<i;j++) xadd.grow(  -lu_i[j]*rhs[j]);
                            rhs[i] = xadd.reduce();
                        }
                        else
                        {
                            const inside_type sum = xadd.reduce();
                            if( abs_of(sum) > scalar0) ii=i;
                            rhs[i] = sum;
                        }
                    }
                }

                for(size_t i=n;i>0;--i) {
                    const readable<inside_type> &lu_i = lu[i];
                    xadd.free();
                    xadd.grow(rhs[i]);
                    for(size_t j=n;j>i;--j) xadd.grow( -lu_i[j]*rhs[j] );
                    rhs[i] = xadd.reduce()/lu_i[i];
                }
            }

            //! compute result from rhs
            template <typename U>
            void solve(const readable<U> &rhs)
            {
                const size_t n = rhs.size(); assert(n<=nmax);
                for(size_t i=n;i>0;--i) result[i] = inside::send(rhs[i]);
                apply(n);
            }

            //! convert result into array
            template <typename ARRAY>
            inline void save(ARRAY &u) const
            {
                const size_t n = u.size(); assert(n<=nmax);
                for(size_t i=n;i>0;--i) u[i] = inside::recv(result[i]);
            }

            //! input => result => solve => output
            template <typename U, typename V>
            inline void solve(writable<U>       &output,
                              const readable<V> &input)
            {
                solve(input);
                save(output);
            }

            //! solve all columns of rhs matrix
            template <typename U>
            void solve(matrix<U> &rhs)
            {
                assert(rhs.rows<=nmax);
                const size_t n = rhs.rows;
                for(size_t j=rhs.cols;j>0;--j)
                {
                    for(size_t i=n;i>0;--i)
                        result[i] = inside::send(rhs[i][j]);
                    solve(n);
                    for(size_t i=n;i>0;--i)
                        rhs[i][j] = inside::recv(result[i]);
                }
            }

            //! compute inverse of decomposed into rhs
            inline void inverse(matrix<T> &rhs)
            {
                assert(rhs.rows<=nmax);
                const size_t n = rhs.rows;
                for(size_t j=n;j>0;--j)
                {
                    // prepare result = delta_ij
                    result.ld(inside0); result[j] = inside1;

                    // apply to result
                    apply(n);

                    // upload result to rhs
                    for(size_t i=n;i>0;--i)
                        rhs[i][j] = inside::recv(result[i]);
                }
            }


            //! inside_type determinant from decomposed
            inside_type det(const size_t n)
            {
                xmul.free();
                const matrix_type &lu = *this;
                for(size_t i=n;i>0;--i) xmul.insert(lu[i][i]);
                return dneg ? -xmul.reduce() : xmul.reduce();
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            xadd_type                               xadd; //!< extended add
            xmul_type                               xmul; //!< extended mul
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(LU);

            thin_array<size_t>                      indx;
            const memory::operative_of<scalar_type> smem;
            thin_array<scalar_type>                 scal;
            const memory::operative_of<inside_type> xmem;

        public:
            thin_array<inside_type>                 result;  //!< result array
            const scalar_type                       scalar0; //!< 0 as scalar
            const scalar_type                       scalar1; //!< 1 as scalar
            const inside_type                       inside0; //!< 0 as inside
            const inside_type                       inside1; //!< 1 as inside

        private:

            //! copy and scale user's matrix
            template <typename U>
            inline bool initialize(const matrix<U> &a)
            {
                const size_t n = a.rows;
                coerce(dneg) = false;
                for(size_t i=n;i>0;--i)
                {
                    scalar_type           piv = scalar0;
                    const readable<U>     &a_i = a[i];
                    writable<inside_type> &d_i = (*this)[i];

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

            //! Crout's algoritm
            inline bool decomposed(const size_t n)
            {
                matrix<inside_type> &dcmp = *this;
                for(size_t j=1;j<=n;++j)
                {
                    for(size_t i=1;i<j;i++) {
                        writable<inside_type> &d_i = dcmp[i];
                        xadd.free();
                        xadd.grow(d_i[j]);
                        for(size_t k=1;k<i;k++)
                        {
                            xadd.grow( -d_i[k] * dcmp[k][j] );
                        }
                        d_i[j] = xadd.reduce();
                    }

                    scalar_type piv = scalar0;
                    size_t      imx = j;
                    for (size_t i=j;i<=n;i++)
                    {
                        writable<inside_type> &d_i = dcmp[i];
                        xadd.free();
                        xadd.grow(d_i[j]);
                        for (size_t k=1;k<j;k++)
                        {
                            xadd.grow( -d_i[k]*dcmp[k][j] );
                        }
                        const scalar_type tmp = scal[i] * abs_of(d_i[j]=xadd.reduce());
                        if(tmp>piv)
                        {
                            piv = tmp;
                            imx = i;
                        }
                    }

                    if(j!=imx)
                    {
                        dcmp.swap_rows(j,imx);
                        scal[j]      = scal[imx];
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

