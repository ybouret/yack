
namespace yack
{

    namespace math
    {

        template <>
        cholesky<real_t>:: ~cholesky() throw()
        {
        }

        template <>
        cholesky<real_t>:: cholesky() throw() : base_type()
        {
        }

        template <>
        cholesky<real_t>:: cholesky(size_t n) : base_type(n,0)
        {
        }

        template <>
        bool cholesky<real_t>:: build( matrix<real_t> &a )
        {
            assert(a.is_square());
            const size_t      n = a.rows;adjust(n,0);
            writable<real_t> &p = *this; assert(n==p.size());

            for (size_t i=1;i<=n;++i)
            {
                for (size_t j=i;j<=n;++j)
                {
                    real_t sum = a[i][j];
                    for(size_t k=i-1;k>0;--k)
                        sum -= a[i][k]*a[j][k];
                    if(i==j)
                    {
                        if(sum<=0) return false;
                        p[i] = sqrt(sum);
                    }
                    else
                    {
                        a[j][i] = sum/p[i];
                    }
                }

            }


            return true;
        }

        template <>
        void cholesky<real_t>:: solve(writable<real_t> &x, const matrix<real_t> &a, const readable<real_t> &b) const throw()
        {
            assert(x.size()==size());
            assert(b.size()==size());
            assert(a.rows  ==size());
            assert(a.cols  ==size());

            const size_t            n = size();
            const readable<real_t> &p = *this; assert(n==p.size());

            for(size_t i=1;i<=n;++i)
            {
                real_t sum=b[i];
                for(size_t k=i-1;k>=1;--k)
                    sum -= a[i][k]*x[k];
                x[i]=sum/p[i];
            }
            for(size_t i=n;i>=1;--i)
            {
                real_t sum = x[i];
                for(size_t k=i+1;k<=n;++k)
                    sum -= a[k][i]*x[k];
                x[i]=sum/p[i];
            }
        }



    }

}
