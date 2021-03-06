
namespace yack
{

    namespace math
    {

        template <>
        svd<real_t>:: ~svd() throw()
        {
        }

        template <>
        svd<real_t>:: svd(const size_t dims) :
        tableaux(1,dims),
        rv1( next() )
        {
        }


        template <>
        bool svd<real_t>:: build(matrix<real_t>   &a,
                                 writable<real_t> &w,
                                 matrix<real_t>   &v)
        {
            static const real_t one(1);
            static const real_t two(2);
            const size_t m = a.rows;
            const size_t n = a.cols;
            assert(v.cols==a.cols);
            assert(v.rows==a.cols);
            assert(w.size()==a.cols);
            ensure(n);
            size_t k;
            real_t c,f,h,s,x,y,z;
            size_t l=0;
            real_t g=0,scale=0,anorm=0;
            /* Householder reduction to bidiagonal form */
            for(size_t i=1;i<=n;i++)
            {
                l=i+1;
                rv1[i]=scale*g;
                g=s=scale=0;
                if (i <= m)
                {
                    for(k=i;k<=m;++k)
                    {
                        scale += std::abs(a[k][i]);
                    }
                    if(scale>0)
                    {
                        for(k=i;k<=m;k++)
                        {
                            a[k][i] /= scale;
                            s += a[k][i]*a[k][i];
                        }
                        f = a[i][i];
                        g = -__sgn(sqrt(s),f);
                        h = f*g-s;
                        a[i][i]=f-g;
                        for(size_t j=l;j<=n;++j)
                        {
                            for(s=0,k=i;k<=m;++k)
                                s += a[k][i]*a[k][j];
                            f=s/h;
                            for(k=i;k<=m;++k)
                                a[k][j] += f*a[k][i];
                        }
                        for (k=i;k<=m;++k) a[k][i] *= scale;
                    }
                }
                w[i]=scale *g;
                g=s=scale=0.0;
                if (i <= m && i != n)
                {
                    for(k=l;k<=n;++k)
                    {
                        scale += std::abs(a[i][k]);
                    }
                    if (scale>0)
                    {
                        for(k=l;k<=n;++k)
                        {
                            a[i][k] /= scale;
                            s += a[i][k]*a[i][k];
                        }
                        f=a[i][l];
                        g = - __sgn(sqrt(s),f);
                        h=f*g-s;
                        a[i][l]=f-g;
                        for(k=l;k<=n;k++)
                            rv1[k]=a[i][k]/h;
                        for(size_t j=l;j<=m;++j) {
                            for (s=0,k=l;k<=n;k++)
                                s += a[j][k]*a[i][k];
                            for(k=l;k<=n;k++)
                                a[j][k] += s*rv1[k];
                        }
                        for (k=l;k<=n;k++)
                            a[i][k] *= scale;
                    }
                }
                anorm = max_of<real_t>(anorm,(std::abs(w[i])+std::abs(rv1[i])));
            }
            for(size_t i=n;i>=1;--i)
            {
                if (i<n)
                {
                    if(std::abs(g)>0)
                    {
                        for(size_t j=l;j<=n;++j)
                            v[j][i]=(a[i][j]/a[i][l])/g;
                        for(size_t j=l;j<=n;j++)
                        {
                            for (s=0,k=l;k<=n;++k)
                                s += a[i][k]*v[k][j];
                            for (k=l;k<=n;++k)
                                v[k][j] += s*v[k][i];
                        }
                    }
                    for(size_t j=l;j<=n;j++)
                        v[i][j]=v[j][i]=0;
                }
                v[i][i]=1.0;
                g=rv1[i];
                l=i;
            }
            for(size_t i=min_of(m,n);i>=1;i--)
            {
                /* Accumulation of left-hand transformations. */
                l=i+1;
                g=w[i];
                for(size_t j=l;j<=n;j++)
                    a[i][j]=0.0;
                if(std::abs(g)>0)
                {
                    g=one/g;
                    for(size_t j=l;j<=n;j++)
                    {
                        for (s=0,k=l;k<=m;k++) s += a[k][i]*a[k][j];
                        f=(s/a[i][i])*g;
                        for (k=i;k<=m;k++) a[k][j] += f*a[k][i];
                    }
                    for(size_t j=i;j<=m;j++) a[j][i] *= g;
                }
                else
                    for(size_t j=i;j<=m;j++) a[j][i]=0;
                ++a[i][i];
            }
            for (k=n;k>=1;k--)
            {
                /* Diagonalization of the bidiagonal form. */
                unsigned its=0;
                for(its=1;its<=MAX_ITS;its++)
                {
                    int   flag = 1;
                    size_t nm  = 0;
                    for (l=k;l>=1;l--)
                    {
                        /* Test for splitting. */
                        nm=l-1;

                        /* Note that rv1[1] is always zero. */
                        if ((std::abs(rv1[l])+anorm) == anorm)
                        {
                            flag=0;
                            break;
                        }
                        if ((std::abs(w[nm])+anorm) == anorm) break;
                    }
                    if (flag)
                    {
                        c=0; /* Cancellation of rv1[l], if l > 1. */
                        s=1;
                        for(size_t i=l;i<=k;i++) {
                            f=s*rv1[i];
                            rv1[i]=c*rv1[i];
                            if ((std::abs(f)+anorm) == anorm) break;
                            g=w[i];
                            h=hypothenuse(f,g);
                            w[i]=h;
                            h=one/h;
                            c=g*h;
                            s = -f*h;
                            for(size_t j=1;j<=m;j++) {
                                y=a[j][nm];
                                z=a[j][i];
                                a[j][nm]=y*c+z*s;
                                a[j][i]=z*c-y*s;
                            }
                        }
                    }
                    z=w[k];
                    if (l == k) { /* Convergence. */
                        if(z<0) { /* Singular value is made nonnegative. */
                            w[k] = -z;
                            for(size_t j=1;j<=n;j++) v[j][k] = -v[j][k];
                        }
                        break;
                    }
                    if (its >= MAX_ITS)
                    {
                        return false; // no convergence
                    }
                    x=w[l]; /* Shift from bottom 2-by-2 minor. */
                    nm=k-1;
                    y=w[nm];
                    g=rv1[nm];
                    h=rv1[k];
                    f=((y-z)*(y+z)+(g-h)*(g+h))/(two*h*y);
                    g=hypothenuse(f,one);
                    f=((x-z)*(x+z)+h*((y/(f+__sgn(g,f)))-h))/x;
                    c=s=one; /* Next QR transformation: */
                    for(size_t j=l;j<=nm;j++)
                    {
                        const size_t i=j+1;
                        g=rv1[i];
                        y=w[i];
                        h=s*g;
                        g=c*g;
                        z=hypothenuse(f,h);
                        rv1[j]=z;
                        c=f/z;
                        s=h/z;
                        f=x*c+g*s;
                        g=g*c-x*s;
                        h=y*s;
                        y *= c;
                        for(size_t jj=1;jj<=n;jj++)
                        {
                            x=v[jj][j];
                            z=v[jj][i];
                            v[jj][j]=x*c+z*s;
                            v[jj][i]=z*c-x*s;
                        }
                        z=hypothenuse(f,h);
                        w[j]=z; /* Rotation can be arbitrary if z = 0. */
                        if(z>0) {
                            z=one/z;
                            c=f*z;
                            s=h*z;
                        }
                        f=c*g+s*y;
                        x=c*y-s*g;
                        for(size_t jj=1;jj<=m;++jj)
                        {
                            y=a[jj][j];
                            z=a[jj][i];
                            a[jj][j]=y*c+z*s;
                            a[jj][i]=z*c-y*s;
                        }
                    }
                    rv1[l]=0.0;
                    rv1[k]=f;
                    w[k]=x;
                }
            }
            return true;
        }


        template <>
        void svd<real_t>::solve(const matrix<real_t>   &u,
                                const readable<real_t> &w,
                                const matrix<real_t>   &v,
                                writable<real_t>       &x,
                                const readable<real_t> &b)
        {
            const size_t m =  u.rows;
            const size_t n =  u.cols;
            assert(v.cols  == u.cols);
            assert(v.rows  == u.cols);
            assert(w.size()== u.cols);
            assert(b.size()==u.rows);
            assert(x.size()==u.cols);

            ensure(n);
            for (size_t j=n;j>0;--j)
            {
                real_t       sum = 0;
                const real_t den = w[j];
                if(std::abs(den)>0)
                {
                    for(size_t i=m;i>0;--i) sum += u[i][j]*b[i];
                    sum /= den;
                }
                rv1[j] = sum;
            }

            for(size_t j=n;j>0;--j)
            {
                real_t sum = 0;
                for(size_t i=n;i>0;--i) sum += v[j][i]*rv1[i];
                x[j]=sum;
            }

        }

#if 0
        template <>
        size_t svd<real_t>:: nullity(writable<real_t> &w, const real_t ftol) throw()
        {
            const size_t n = w.size();
            if(n>0)
            {
                //--------------------------------------------------------------
                //
                // first pass: find wmax
                //
                //--------------------------------------------------------------
                real_t wmax = fabs(w[n]);
                {
                    size_t i=n;
                    while(--i>0)
                    {
                        wmax = max_of<real_t>( fabs(w[i]), wmax);
                    }
                }

                //--------------------------------------------------------------
                //
                // second pass: remove smallest components
                //
                //--------------------------------------------------------------
                size_t     res=0;
                for(size_t i=n;i>0;--i)
                {
                    real_t &ww = w[i];
                    if( fabs(ww) <= fabs(ftol*wmax) )
                    {
                        ww = 0;
                        ++res;
                    }
                }
                return res;
            }
            else
            {
                return 0;
            }
        }
#endif
    }

}
