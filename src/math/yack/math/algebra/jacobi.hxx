
namespace yack
{
    namespace math
    {

        template <>
        jacobi<real_t>:: ~jacobi() throw()
        {
        }

        template <>
        jacobi<real_t>:: jacobi(const size_t n) :
        tableaux(2,n),
        b( next() ),
        z( next() )
        {
        }


        template < >
        void   jacobi<real_t>:: eigsrt(writable<real_t> &d, matrix<real_t> &v) throw()
        {
            const size_t n = v.rows;
            assert( d.size() >= v.rows );
            for (size_t i=1,ip=2;i<n;++i,++ip)
            {
                size_t k = i;
                real_t p = d[k];
                for (size_t j=ip;j<=n;j++)
                {
                    if (d[j] >= p)
                    {
                        p=d[k=j];
                    }
                }
                if (k != i)
                {
                    d[k]=d[i];
                    d[i]=p;
                    v.swap_cols(i,k);
                }
            }
        }

        template <>
        void   jacobi<real_t>:: eigsrtA(writable<real_t> &d, matrix<real_t> &v) throw()
        {
            const size_t n = v.rows;
            assert( d.size() >= v.rows );
            for (size_t i=1,ip=2;i<n;++i,++ip)
            {
                size_t k = i;
                real_t p = d[k];
                for (size_t j=ip;j<=n;j++)
                {
                    if ( fabs(d[j]) >= fabs(p) )
                    {
                        p=d[k=j];
                    }
                }
                if (k != i)
                {
                    d[k]=d[i];
                    d[i]=p;
                    v.swap_cols(i,k);
                }
            }
        }

        template <>
        bool  jacobi<real_t>:: operator()( matrix<real_t> &a, writable<real_t> &d, matrix<real_t> &v, sort_eigv_by kind )
        {
            static const real_t half(0.5);
            static const real_t one(1);
            static const real_t hundred(100);

            assert(a.rows>0);
            assert(a.is_square());
            assert(matrix<real_t>::have_same_sizes(a,v));
            assert(d.size() == a.rows);

            bool           success = true;
            const size_t   n       = a.rows;
            ensure(n);

            //==============================================================
            // initialize eigenvectors and workspace
            //==============================================================
            for(size_t ip=1;ip<=n;++ip)
            {
                {
                    writable<real_t> &v_ip = v[ip];
                    for(size_t iq=1;iq<=n;++iq) v_ip[iq]=0;
                    v_ip[ip]=1;
                }
                writable<real_t> &a_ip = a[ip];
                b[ip]=d[ip]=a_ip[ip];
                z[ip]=0;
            }

            //==============================================================
            // looping over sweeps
            //==============================================================
            for(size_t iter=1;iter<=max_iter;++iter)
            {
                real_t sm = 0;
                for(size_t ip=1;ip<n;++ip) {
                    for(size_t iq=ip+1;iq<=n; ++iq)
                        sm += fabs(a[ip][iq]);
                }
                if (sm <= numeric<real_t>::minimum )
                {
                    assert(true==success);
                    goto DONE; // OK
                }

                const real_t tresh = (iter<min_iter) ? static_cast<real_t>(0.2)*sm/(n*n) : 0;


                for(size_t ip=1;ip<n;++ip)
                {
                    for(size_t iq=ip+1;iq<=n;++iq)
                    {
                        real_t g = hundred * fabs(a[ip][iq]);
                        if ( (iter>min_iter) && almost_equal( fabs(d[ip])+g, fabs(d[ip]))
                            && almost_equal( fabs(d[iq])+g, fabs(d[iq])) )
                        {
                            a[ip][iq]=0;
                        }
                        else
                            if (fabs(a[ip][iq]) > tresh)
                            {
                                real_t h = d[iq]-d[ip];
                                real_t t = 0;
                                if ( almost_equal(fabs(h)+g,fabs(h)) )
                                {
                                    t=(a[ip][iq])/h;
                                }
                                else
                                {
                                    const real_t theta= half*h/(a[ip][iq]);
                                    t=one/(fabs(theta)+sqrt(one+theta*theta));
                                    if (theta < 0)
                                        t = -t;
                                }
                                const real_t c   = one/sqrt(one+t*t);
                                const real_t s   = t*c;
                                const real_t tau = s/(one+c);
                                h=t*a[ip][iq];
                                z[ip] -= h;
                                z[iq] += h;
                                d[ip] -= h;
                                d[iq] += h;
                                a[ip][iq]=0;
                                for(size_t j=1;j<ip;++j) {
                                    _Y_JACOBI(a,j,ip,j,iq);
                                }
                                for(size_t j=ip+1;j<iq;++j) {
                                    _Y_JACOBI(a,ip,j,j,iq);
                                }
                                for(size_t j=iq+1;j<=n;++j) {
                                    _Y_JACOBI(a,ip,j,iq,j);
                                }
                                for(size_t j=1;j<=n;++j) {
                                    _Y_JACOBI(v,j,ip,j,iq);
                                }
                            }
                    }
                }

                for(size_t ip=1;ip<=n;++ip)
                {
                    b[ip] += z[ip];
                    d[ip] = b[ip];
                    z[ip] = 0;
                }
            }
            success = false;

        DONE:
            //__________________________________________________________________
            //
            // matrix regeneration
            //__________________________________________________________________
            for(size_t i=1,ip=2;i<n;++i,++ip)
            {
                for(size_t j=ip;j<=n;++j)
                {
                    a[i][j] = a[j][i];
                }
            }
            if(success)
            {
                switch(kind)
                {
                    case sort_eigv_none:                    break;
                    case sort_eigv_by_value:  eigsrt(d,v);  break;
                    case sort_eigv_by_module: eigsrtA(d,v); break;
                }
                return true;
            }
            else
            {
                return false;
            }

        }


    }

}
