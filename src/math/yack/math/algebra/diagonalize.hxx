
namespace yack
{

    namespace math
    {

        template <>
        void diagonalize::Hessenberg::Balance<real_t>( matrix<real_t> &a ) noexcept
        {
            static const real_t RADIX = 2;
            static const real_t sqrdx = RADIX*RADIX;
            static const real_t one(1);
            static const real_t fac(0.95);

            assert( a.is_square() );
            assert( a.rows>0      );
            const size_t n = a.rows;

            size_t last=0;
            while(0==last)
            {
                last=1;
                for(size_t i=1;i<=n;i++)
                {
                    real_t r=0,c=0;
                    for (size_t j=1;j<=n;j++)
                        if (j != i)
                        {
                            c += std::abs(a[j][i]);
                            r += std::abs(a[i][j]);
                        }
                    if( (c>0) && (r>0) )
                    {
                        real_t g=r/RADIX;
                        real_t f=one;
                        real_t s=c+r;
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
                        if( (c+r)/f < fac*s)
                        {
                            last=0;
                            g=one/f;
                            for(size_t j=1;j<=n;j++) a[i][j] *= g;
                            for(size_t j=1;j<=n;j++) a[j][i] *= f;
                        }
                    }
                }
            }
        }

    }
}

namespace yack
{

    namespace math
    {
        template < >
        void diagonalize:: Hessenberg:: Reduce<real_t>( matrix<real_t> &a ) noexcept
        {
            assert(a.is_square());
            const size_t n = a.rows;

            //------------------------------------------------------------------
            // m = r+1
            //------------------------------------------------------------------
            for(size_t m=2; m<n; ++m )
            {
                const size_t r   = m-1;
                real_t       piv = 0;
                size_t       s   = m;
                for( size_t j=m+1;j<=n;++j)
                {
                    //----------------------------------------------------------
                    // find the pivot
                    //----------------------------------------------------------
                    const real_t tmp = a[j][r];
                    if(std::abs(tmp)>std::abs(piv))
                    {
                        piv = tmp;
                        s   = j;
                    }
                }
                if( s != m )
                {
                    //std::cerr << "\t#SWAP(" << s << "," << m << ")" <<  "/pivot=" << piv << std::endl;
                    assert(std::abs(piv)>0);
                    //----------------------------------------------------------
                    // First similarity transform: exchange colums/rows
                    //----------------------------------------------------------
                    a.swap_both(s,m);

                    //----------------------------------------------------------
                    // Second similarity transform
                    //----------------------------------------------------------
                    assert( std::abs(piv-a[m][m-1]) <= 0 );
                    for(size_t i=m+1;i<=n;++i)
                    {
                        const real_t factor = a[i][r] / piv;

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

    }

}


namespace yack
{
    namespace math
    {

        template <>
        bool diagonalize:: Hessenberg:: QR<real_t>( matrix<real_t> &a, writable<real_t> &wr, writable<real_t> &wi, size_t &nr) noexcept
        {
            assert( a.is_square() );
            assert( a.rows>0      );
            const ptrdiff_t n = a.rows;
            assert( wr.size()   >= a.rows );
            assert( wi.size()   >= a.rows );

            ptrdiff_t nn,m,l,k,j,i,mmin;
            real_t    z,y,x,w,v,u,t,s,r=0,q=0,p=0,anorm;

            size_t   ir = 1; //! where to put real eigenvalues
            size_t   ic = n; //! where to put cplx eigenvalues
            nr    = 0;
            anorm = 0;
            for (i=1;i<=n;i++)
                for (j=max_of<size_t>(i-1,1);j<=n;j++)
                    anorm += std::abs(a[i][j]);
            nn=n;
            t=0;
            while(nn>=1)
            {
                unsigned its=0;
                do
                {
                    for (l=nn;l>=2;l--)
                    {
                        s=std::abs(a[l-1][l-1])+std::abs(a[l][l]);
                        if (s <= 0)
                            s=anorm;
                        if ((real_t)(std::abs(a[l][l-1]) + s) == s)
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
                            p=static_cast<real_t>(0.5)*(y-x);
                            q=p*p+w;
                            z=sqrt(std::abs(q));
                            x += t;
                            if (q >= 0)
                            {
                                z=p+__sgn(z,p);
                                wr[ir+1]=wr[ir]=x+z;
                                if( std::abs(z)>0 )
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
                                s=std::abs(a[nn][nn-1])+std::abs(a[nn-1][nn-2]);
                                y=x= static_cast<real_t>(0.75)*s;
                                w  = static_cast<real_t>(-0.4375)*s*s;
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
                                s=std::abs(p)+std::abs(q)+std::abs(r);
                                p /= s;
                                q /= s;
                                r /= s;
                                if (m == l)
                                {
                                    break;
                                }
                                u=std::abs(a[m][m-1])*(std::abs(q)+std::abs(r));
                                v=std::abs(p)*(std::abs(a[m-1][m-1])+std::abs(z)+std::abs(a[m+1][m+1]));
                                if ((real_t)(u+v) == v)
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
                                    if ( (x=std::abs(p)+std::abs(q)+std::abs(r))>0 )
                                    {
                                        p /= x;
                                        q /= x;
                                        r /= x;
                                    }
                                }
                                if( std::abs(s=__sgn(sqrt(p*p+q*q+r*r),p)) > 0 )
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
            hsort(wr,nr,comparison::increasing<real_t>);
            return true;
        }

    }
}


namespace yack
{
    namespace math
    {
        //! all in one eigenvalues finding....
        template < >
        bool diagonalize:: eig<real_t>( matrix<real_t> &a, writable<real_t> &wr, writable<real_t> &wi, size_t &nr) noexcept
        {
            Hessenberg::Balance(a);
            Hessenberg::Reduce(a);
            return Hessenberg::QR(a,wr,wi,nr);
        }
    }

}

