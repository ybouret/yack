
namespace yack
{

    namespace math
    {

        template <>
        svd<real_t>:: ~svd() noexcept
        {
        }

        template <>
        svd<real_t>:: svd() :
        zero(0),
        one(1),
        two(2),
        rv1(),
        add()
        {
        }

        template <>
        bool svd<real_t>:: build(matrix<xtype>   &a,
                                 writable<xtype> &w,
                                 matrix<xtype>   &v)
        {
            const size_t m = a.rows;
            const size_t n = a.cols;
            assert(v.cols==n);
            assert(v.rows==n);
            assert(w.size()==n);



            rv1.adjust(n,zero);
            size_t k;
            xtype  c,f,h,s,x,y,z;
            size_t l = 0;
            xtype g=zero,scale=zero,anorm=zero;

            /* Householder reduction to bidiagonal form */
            for(size_t i=1;i<=n;i++)
            {
                l=i+1;
                rv1[i]=scale*g;
                g=s=scale=zero;
                add.free();
                if (i <= m)
                {
                    for(k=i;k<=m;k++)
                        add.push(a[k][i].abs());
                    scale=add.reduce();

                    if(scale>0)
                    {
                        add.free();
                        for(k=i;k<=m;k++)
                        {
                            a[k][i] /= scale;
                            add.push( squared(a[k][i]) );
                        }
                        s = add.reduce();
                        f = a[i][i];
                        g = -sgn(sqrt_of(s),f);
                        h = f*g-s;
                        a[i][i]=f-g;
                        for(size_t j=l;j<=n;j++)
                        {
                            add.free();
                            for(k=i;k<=m;k++)
                                add.push( a[k][i]*a[k][j] );
                            s = add.reduce();
                            f = s/h;
                            for(k=i;k<=m;k++)
                                a[k][j] += f*a[k][i];
                        }
                        for (k=i;k<=m;k++) a[k][i] *= scale;
                    }
                }

                w[i]=scale *g;
                g=s=scale=zero;
                if (i <= m && i != n)
                {
                    add.free();
                    for(k=l;k<=n;k++)
                        add.push(a[i][k].abs());
                    scale = add.reduce();

                    if(scale>0)
                    {
                        add.free();
                        for(k=l;k<=n;k++)
                        {
                            a[i][k] /= scale;
                            add.push( squared(a[i][k]));
                        }
                        s = add.reduce();
                        f = a[i][l];
                        g = -sgn(sqrt_of(s),f);
                        h=f*g-s;
                        a[i][l]=f-g;
                        for(k=l;k<=n;k++)
                            rv1[k]=a[i][k]/h;
                        for(size_t j=l;j<=m;j++)
                        {
                            add.free();
                            for(k=l;k<=n;k++)
                                add.push(a[j][k]*a[i][k]);
                            s = add.reduce();
                            for(k=l;k<=n;k++)
                                a[j][k] += s*rv1[k];
                        }
                        for (k=l;k<=n;k++)
                            a[i][k] *= scale;
                    }
                }
                anorm = max_of(anorm,(abs_of(w[i])+abs_of(rv1[i])));
            }
            
            for(size_t i=n;i>=1;i--)
            {
                /* Accumulation of right-hand transformations. */
                if (i<n)
                {
                    if(g.abs()>zero)
                    {
                        for(size_t j=l;j<=n;j++)
                            v[j][i]=(a[i][j]/a[i][l])/g;

                        for(size_t j=l;j<=n;j++)
                        {
                            add.free();
                            for(k=l;k<=n;k++)
                                //s += a[i][k]*v[k][j];
                                add.push(a[i][k]*v[k][j]);
                            s = add.reduce();

                            for (k=l;k<=n;k++)
                                v[k][j] += s*v[k][i];
                        }
                    }
                    for(size_t j=l;j<=n;j++)
                        v[i][j]=v[j][i]=zero;
                }
                v[i][i]=one;
                g=rv1[i];
                l=i;
            }

            for(size_t i=min_of(m,n);i>0;--i)
            {
                /* Accumulation of left-hand transformations. */
                l=i+1;
                g=w[i];
                for(size_t j=l;j<=n;j++)
                    a[i][j]=0.0;
                if(abs_of(g)>0)
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
                    for(size_t j=i;j<=m;j++) a[j][i]=0.0;
                a[i][i] += one;
            }

            for (k=n;k>0;--k)
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
                        if( (abs_of(rv1[l])+anorm) == anorm)
                        {
                            flag=0;
                            break;
                        }
                        if( (abs_of(w[nm])+anorm) == anorm) break;
                    }
                    if (flag)
                    {
                        c=0; /* Cancellation of rv1[l], if l > 1. */
                        s=1;
                        for(size_t i=l;i<=k;i++) {
                            f=s*rv1[i];
                            rv1[i]=c*rv1[i];
                            if ( (abs_of(f)+anorm) == anorm) break;
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
                    f=((x-z)*(x+z)+h*((y/(f+sgn(g,f)))-h))/x;
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
                        for(size_t jj=1;jj<=m;jj++)
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
        void svd<real_t>:: solve(const  matrix<xtype>   &u,
                                 const  readable<xtype> &w,
                                 const  matrix<xtype>   &v,
                                 writable<xtype>        &x,
                                 const readable<xtype>  &b)
        {
            const size_t m = u.rows;
            const size_t n = u.cols;
            assert(v.cols==n);
            assert(v.rows==n);
            assert(w.size()==n);
            assert(b.size()==m);
            assert(x.size()==n);

            rv1.adjust(n,zero);
            add.resume(n);

            //------------------------------------------------------------------
            //
            // calculate (1/w) * u' * b
            //
            //------------------------------------------------------------------
            for(size_t j=n;j>0;--j)
            {
                xtype       s   = zero;
                const xtype den = w[j];
                if(den.abs()>zero)
                {
                    add.free();
                    for(size_t i=m;i>0;--i)
                    {
                        const xtype p = u[i][j] * b[i];
                        add.push(p);
                    }
                    s = add.reduce()/den;
                }
                rv1[j] = s;
            }

            //------------------------------------------------------------------
            //
            // multiply by V
            //
            //------------------------------------------------------------------
            for(size_t j=n;j>0;--j)
            {
                add.free();
                const readable<xtype> &v_j = v[j];
                for(size_t i=n;i>0;--i)
                {
                    const xtype p = v_j[i] * rv1[i];
                    add.push(p);
                }
                x[j] = add.reduce();
            }

        }
      



    }

}
