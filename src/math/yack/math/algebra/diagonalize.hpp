//! \file
#ifndef YACK_MATH_DIAGONALIZE_INCLUDED
#define YACK_MATH_DIAGONALIZE_INCLUDED 1

#include "yack/math/numeric.hpp"
#include "yack/math/look-for.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/type/utils.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/exception.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/container/matrix.hpp"
#include <cmath>

namespace yack
{
    
    namespace math {
        
        //! routines to diagonalize a real matrix
        struct diagonalize
        {
            //! Hessenberg algorithms
            struct Hessenberg
            {
                //! balance a real matrix
                /**
                 Given a matrix a[1..n][1..n], this routine replaces it by a balanced matrix with identical eigenvalues.
                 A symmetric matrix is already balanced and is unaffected by this procedure.
                 */
                template <typename T> static void Balance( matrix<T> &a ) noexcept;

                //! reduction to a real Hessenberg form
                /**
                 matrix should be balanced.
                 */
                template <typename T> static void Reduce( matrix<T> &a ) noexcept;

                static const unsigned MAX_ITS = 100; //!< maximum number of cycles
                static const unsigned SCALING =  10; //!< scaling every cycle

                //! find the eigen values
                /**
                 \param a  a real matrix reduced to its Hessenberg form: destructed !
                 \param wr an array that will be filled with the real parts
                 \param wi an array that will be filled with the imagnary parts
                 \param nr the number or real eigenvalues
                 wi[1..nr]=0 and wr[1..nr] are sorted by increasing order.
                 */
                template <typename T> static inline
                bool QR(matrix<T> &a, writable<T> &wr, writable<T> &wi, size_t &nr) noexcept;
                
            };
            
            //! all in one eigenvalues finding....
            template <typename T> static
            bool eig(matrix<T> &a, writable<T> &wr, writable<T> &wi, size_t &nr) noexcept;
            
            
            //! compare by absolute real type value
            template <typename T> static inline
            int __compare_fabs(const T lhs, const T rhs) noexcept
            {
                const T L = std::abs(lhs);
                const T R = std::fabs(rhs);
                return comparison::increasing(L,R);
            }

#if 0
            //! find the eigenvectors from initial eigenvalues
            /**
             TODO: need to be written again ?
             \param ev transpose eigenvectors: e.vrows <= number of REAL eigenvalues
             \param A  original matrix
             \param wr initial eigenvalues, 1..ev.rows are REAL. MUST be SORTED
             \param s  a svd object
             */
            template <typename T> static inline
            bool eigv(matrix<T>       &ev,
                      const matrix<T> &A,
                      writable<T>     &wr,
                      svd<T>          &s)
            {
                assert( A.is_square() );
                assert( wr.size() >= ev.rows );
                
                const size_t n  = A.rows;
                const size_t nv = ev.rows;
                assert(ev.cols==n);
                
                matrix<T>      B(n,n); // A - tau * Id
                matrix<T>      U(n,n); // for SVD
                matrix<T>      V(n,n); // for SVD
                vector<T>      W(n);   // for SVD
                vector<size_t> J(n);   // for indexing |W|
                
                vector<T> y(n);
                vector<T> z(n);
                
                ev.ld(0);
                
                for(size_t iv=1; iv <= nv; )
                {
                    //==========================================================
                    //
                    // Check null space in SVD
                    //
                    //==========================================================
                    size_t nz = 0;
                    while(true)
                    {
                        //------------------------------------------------------
                        // B = A - wr[iv]*Id
                        //------------------------------------------------------
                        B.assign(A);
                        for(size_t i=n;i>0;--i)
                        {
                            B[i][i] -= wr[iv];
                        }
                        
                        //------------------------------------------------------
                        // B = U * W * V'
                        //------------------------------------------------------
                        U.assign(B);
                        if( !s.build(U, W, V) )
                        {
                            return false;
                        }
                        //nz = __find<T>::truncate(W);
                        nz = look_for<T>::nullity::of(W,0);
                        indexing::make(J, __compare_fabs<T>, W);
                        
                        if(nz>0)
                        {
                            break;
                        }
                        
                        
                        //------------------------------------------------------
                        // inverse power using the smallest singular value
                        //------------------------------------------------------
                        const size_t j0 = J[1]; assert(j0>0); assert(j0<=n);
                        for(size_t i=n;i>0;--i)
                        {
                            y[i] = V[i][j0];
                        }
                        s.solve(U, W, V, y, z);
                        
                        //------------------------------------------------------
                        // improve tau
                        //------------------------------------------------------
                        const T dtau = T(1) / tao::v1::dot<T>::of(y,z);
                        wr[iv] += dtau;
                    }
                    
                    //==========================================================
                    // Compute nullspace
                    //==========================================================
                    assert(nz>0);
                    const T tau = wr[iv];
                    for(size_t k=1;k<=nz;++k)
                    {
                        if(iv>nv)
                        {
                            break;
                        }
                        wr[iv] = tau;
                        writable<T>    &vec = ev[iv];
                        const size_t    j   = J[k]; assert(j>0); assert(j<=n);
                        assert(std::abs(W[j])<=0);
                        for(size_t i=n;i>0;--i)
                        {
                            vec[i] = V[i][j];
                        }
                        ++iv;
                    }
                }
                return true;
            }
#endif
        };
        
    }
}

#endif

