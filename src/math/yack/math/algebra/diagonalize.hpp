//! \file
#ifndef YACK_MATH_DIAGONALIZE_INCLUDED
#define YACK_MATH_DIAGONALIZE_INCLUDED 1

#include "yack/math/algebra/svd.hpp"
#include "yack/math/numeric.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/type/utils.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/exception.hpp"
#include "yack/sort/indexing.hpp"
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
                template <typename T> static inline
                void Balance( matrix<T> &a ) throw()
                {
                    static const T RADIX = T(2);
                    static const T sqrdx = RADIX*RADIX;
                    
                    assert( a.is_square() );
                    assert( a.rows>0      );
                    const size_t n = a.rows;
                    
                    size_t last=0;
                    while(0==last)
                    {
                        last=1;
                        for(size_t i=1;i<=n;i++)
                        {
                            T r=0,c=0;
                            for (size_t j=1;j<=n;j++)
                                if (j != i)
                                {
                                    c += fabs(a[j][i]);
                                    r += fabs(a[i][j]);
                                }
                            if( (c>0) && (r>0) )
                            {
                                T g=r/RADIX;
                                T f=T(1);
                                T s=c+r;
                                while (c<g)
                                {
                                    f *= RADIX;
                                    c *= sqrdx;
                                }
                                g=r*RADIX;
                                while (c>g)
                                {
                                    f /= RADIX;
                                    c /= sqrdx;
                                }
                                if( (c+r)/f < T(0.95)*s)
                                {
                                    last=0;
                                    g=T(1)/f;
                                    for(size_t j=1;j<=n;j++) a[i][j] *= g;
                                    for(size_t j=1;j<=n;j++) a[j][i] *= f;
                                }
                            }
                        }
                    }
                }
                
                //! reduction to a real Hessenberg form
                /**
                 matrix should be balanced.
                 */
                template <typename T> static inline
                void Reduce( matrix<T> &a ) throw()
                {
                    assert(a.is_square());
                    const size_t n = a.rows;
                    
                    //------------------------------------------------------------------
                    // m = r+1
                    //------------------------------------------------------------------
                    for(size_t m=2; m<n; ++m )
                    {
                        const size_t r   = m-1;
                        T            piv = 0;
                        size_t       s   = m;
                        for( size_t j=m+1;j<=n;++j)
                        {
                            //----------------------------------------------------------
                            // find the pivot
                            //----------------------------------------------------------
                            const T tmp = a[j][r];
                            if(fabs(tmp)>fabs(piv))
                            {
                                piv = tmp;
                                s   = j;
                            }
                        }
                        if( s != m )
                        {
                            //std::cerr << "\t#SWAP(" << s << "," << m << ")" <<  "/pivot=" << piv << std::endl;
                            assert(fabs(piv)>0);
                            //----------------------------------------------------------
                            // First similarity transform: exchange colums/rows
                            //----------------------------------------------------------
                            a.swap_both(s,m);
                            
                            //----------------------------------------------------------
                            // Second similarity transform
                            //----------------------------------------------------------
                            assert( fabs(piv-a[m][m-1]) <= 0 );
                            for(size_t i=m+1;i<=n;++i)
                            {
                                const T factor = a[i][r] / piv;
                                
                                //------------------------------------------------------
                                // subtract factor times row r + 1 from row i
                                //------------------------------------------------------
                                for(size_t j=1;j<=n;++j) a[i][j] -= factor * a[m][j];
                                
                                //------------------------------------------------------
                                // add factor times column i to column r + 1
                                //------------------------------------------------------
                                for(size_t j=1;j<=n;j++) a[j][m] += factor * a[j][i];
                            }
                        }
                    }
                    
                    //==================================================================
                    // clean up to the exact Hessenberg form
                    //==================================================================
                    for(size_t j=n;j>0;--j)
                    {
                        for(size_t i=j+2;i<=n;++i)
                            a[i][j] = 0;
                    }
                }
                
                static const unsigned MAX_ITS =100; //!< maximum number of cycles
                static const unsigned SCALING = 10; //!< scaling every cycle
                                                    //! find the eigen values
                /**
                 \param a  a real matrix reduced to its Hessenberg form: destructed !
                 \param wr an array that will be filled with the real parts
                 \param wi an array that will be filled with the imagnary parts
                 \param nr the number or real eigenvalues
                 wi[1..nr]=0 and wr[1..nr] are sorted by increasing order.
                 */
                template <typename T> static inline
                bool QR( matrix<T> &a, writable<T> &wr, writable<T> &wi, size_t &nr) throw()
                {
                    assert( a.is_square() );
                    assert( a.rows>0      );
                    const ptrdiff_t n = a.rows;
                    assert( wr.size()   >= a.rows );
                    assert( wi.size()   >= a.rows );
                    //assert( flag.size() >= n );
                    
                    ptrdiff_t nn,m,l,k,j,i,mmin;
                    T         z,y,x,w,v,u,t,s,r=0,q=0,p=0,anorm;
                    
                    size_t   ir = 1; //! where to put real eigenvalues
                    size_t   ic = n; //! where to put cplx eigenvalues
                    nr    = 0;
                    anorm = 0;
                    for (i=1;i<=n;i++)
                        for (j=max_of<size_t>(i-1,1);j<=n;j++)
                            anorm += fabs(a[i][j]);
                    nn=n;
                    t=0;
                    while(nn>=1)
                    {
                        unsigned its=0;
                        do
                        {
                            for (l=nn;l>=2;l--)
                            {
                                s=fabs(a[l-1][l-1])+fabs(a[l][l]);
                                if (s <= 0)
                                    s=anorm;
                                if ((T)(fabs(a[l][l-1]) + s) == s)
                                    break;
                            }
                            x=a[nn][nn];
                            if (l == nn)
                            {
                                wr[ir]=x+t;
                                wi[ir]=0;
                                //std::cerr << "#EIG: real single: " << wr[ir] << std::endl;
                                ++ir;
                                ++nr;
                                --nn;
                            }
                            else
                            {
                                y=a[nn-1][nn-1];
                                w=a[nn][nn-1]*a[nn-1][nn];
                                if(l == (nn-1))
                                {
                                    p=T(0.5)*(y-x);
                                    q=p*p+w;
                                    z=sqrt(fabs(q));
                                    x += t;
                                    if (q >= 0)
                                    {
                                        z=p+__sgn(z,p);
                                        wr[ir+1]=wr[ir]=x+z;
                                        if( fabs(z)>0 )
                                            wr[ir]=x-w/z;
                                        //std::cerr << "#EIG: real pair: " << wr[ir] << ", " << wr[ir+1] << ", x=" << x << ", w=" << w << ", z=" << z << ", p=" << p << ", sq=" << Sqrt(Fabs(q)) << std::endl;
                                        wi[ir+1]=wi[ir]=0;
                                        ir += 2;
                                        nr += 2;
                                    }
                                    else
                                    {
                                        wr[ic-1]=wr[ic]=x+p;
                                        wi[ic-1]= -(wi[ic]=z);
                                        ic -= 2;
                                    }
                                    nn -= 2;
                                }
                                else
                                {
                                    if (its >= MAX_ITS)
                                    {
                                        return false;
                                    }
                                    if (0 == (its%SCALING) )
                                    {
                                        t += x;
                                        for (i=1;i<=nn;i++)
                                            a[i][i] -= x;
                                        s=fabs(a[nn][nn-1])+fabs(a[nn-1][nn-2]);
                                        y=x= T(0.75)*s;
                                        w = -T(0.4375)*s*s;
                                    }
                                    ++its;
                                    for(m=(nn-2);m>=l;m--)
                                    {
                                        z=a[m][m];
                                        r=x-z;
                                        s=y-z;
                                        p=(r*s-w)/a[m+1][m]+a[m][m+1];
                                        q=a[m+1][m+1]-z-r-s;
                                        r=a[m+2][m+1];
                                        s=fabs(p)+fabs(q)+fabs(r);
                                        p /= s;
                                        q /= s;
                                        r /= s;
                                        if (m == l)
                                        {
                                            break;
                                        }
                                        u=fabs(a[m][m-1])*(fabs(q)+fabs(r));
                                        v=fabs(p)*(fabs(a[m-1][m-1])+fabs(z)+fabs(a[m+1][m+1]));
                                        if ((T)(u+v) == v)
                                        {
                                            break;
                                        }
                                    }
                                    for (i=m+2;i<=nn;i++)
                                    {
                                        a[i][i-2]=0;
                                        if (i != (m+2))
                                            a[i][i-3]=0;
                                    }
                                    for (k=m;k<=nn-1;k++)
                                    {
                                        if (k != m)
                                        {
                                            p=a[k][k-1];
                                            q=a[k+1][k-1];
                                            r=0;
                                            if (k != (nn-1)) r=a[k+2][k-1];
                                            if ( (x=fabs(p)+fabs(q)+fabs(r))>0 )
                                            {
                                                p /= x;
                                                q /= x;
                                                r /= x;
                                            }
                                        }
                                        if( fabs(s=__sgn(sqrt(p*p+q*q+r*r),p)) > 0 )
                                        {
                                            if (k == m)
                                            {
                                                if (l != m)
                                                    a[k][k-1] = -a[k][k-1];
                                            }
                                            else
                                            {
                                                a[k][k-1] = -s*x;
                                            }
                                            p += s;
                                            x=p/s;
                                            y=q/s;
                                            z=r/s;
                                            q /= p;
                                            r /= p;
                                            for(j=k;j<=nn;j++)
                                            {
                                                p=a[k][j]+q*a[k+1][j];
                                                if (k != (nn-1))
                                                {
                                                    p += r*a[k+2][j];
                                                    a[k+2][j] -= p*z;
                                                }
                                                a[k+1][j] -= p*y;
                                                a[k][j]   -= p*x;
                                            }
                                            mmin = nn<k+3 ? nn : k+3;
                                            for (i=l;i<=mmin;i++)
                                            {
                                                p=x*a[i][k]+y*a[i][k+1];
                                                if (k != (nn-1)) {
                                                    p += z*a[i][k+2];
                                                    a[i][k+2] -= p*r;
                                                }
                                                a[i][k+1] -= p*q;
                                                a[i][k] -= p;
                                            }
                                        }
                                    }
                                }
                            }
                        } while (l < nn-1);
                    }
                    hsort(wr,nr,comparison::increasing<T>);
                    return true;
                }
                
            };
            
            //! all in one eigenvalues finding....
            template <typename T> static inline
            bool eig( matrix<T> &a, writable<T> &wr, writable<T> &wi, size_t &nr) throw()
            {
                Hessenberg::Balance(a);
                Hessenberg::Reduce(a);
                return Hessenberg::QR(a, wr, wi, nr);
            }
            
            
            //! compare by absolute real type value
            template <typename T> static inline
            int __compare_fabs(const T lhs, const T rhs) throw()
            {
                const T L = fabs_of(lhs);
                const T R = fabs_of(rhs);
                return comparison::increasing(L,R);
            }
            
            //! find the eigenvectors from initial eigenvalues
            /**
             \param ev transpose eigenvectors: e.vrows <= number of REAL eigenvalues
             \param A  initial matrix
             \param wr initial eigenvalues, 1..ev.rows are REAL. MUST be SORTED
             */
            template <typename T> static inline
            void eigv(matrix<T>       &ev,
                      const matrix<T> &A,
                      writable<T>     &wr,
                      svd<T>          &s)
            {
                assert( A.is_square );
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
                            throw exception("diag::eigv(bad matrix,level-1)");
                        }
                        //nz = __find<T>::truncate(W);
                        nz = svd<T>::nullity(W);
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
                        assert(fabs_of(W[j])<=0);
                        for(size_t i=n;i>0;--i)
                        {
                            vec[i] = V[i][j];
                        }
                        ++iv;
                    }
                }
            }
            
        };
        
    }
}

#endif

