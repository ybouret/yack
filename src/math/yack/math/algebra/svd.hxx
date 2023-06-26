
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

            // calculate u' * b
            for(size_t j=n;j>0;--j)
            {
                xtype s = zero;
                const xtype den = w[j].abs();
                if(den>zero)
                {
                    add.free();
                    for(size_t i=m;i>0;--i)
                    {
                        const xtype p = u[i][j] * b[j];
                        add.push(p);
                    }
                    s = add.reduce()/den;
                }
                rv1[j] = s;
            }

            // multiply by V
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
