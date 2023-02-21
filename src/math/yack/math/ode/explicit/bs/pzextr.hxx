
namespace yack
{
    namespace math
    {

        namespace ode
        {
            template <> pzextr<real_t>:: ~pzextr() noexcept {}

            template <> pzextr<real_t>:: pzextr() : zextr<real_t>(1),
            c(next())
            {
            }

            template <>
            void pzextr<real_t>::operator()(const size_t            iest,
                                            const real_t            xest,
                                            const readable<real_t> &yest,
                                            writable<real_t>       &yz,
                                            writable<real_t>       &dy,
                                            writable<real_t>       &x,
                                            matrix<real_t>         &d)
            {
                assert(yest.size()==yz.size());
                assert(yest.size()==dy.size());
                assert(iest>0);
                assert(iest<=x.size());
                assert(iest<=d.cols);

                const size_t nv = yest.size(); make(nv);

                // update
                x[iest]=xest;
                for(size_t j=nv;j>0;--j)
                    dy[j]=yz[j]=yest[j];

                if (iest == 1) {
                    for(size_t j=nv;j>0;--j)
                        d[j][1]=yest[j];
                }
                else
                {
                    for(size_t j=nv;j>0;--j)
                        c[j]=yest[j];

                    for(size_t k1=1;k1<iest;k1++)
                    {
                        real_t       delta=1/(x[iest-k1]-xest);
                        const real_t f1=xest*delta;
                        const real_t f2=x[iest-k1]*delta;
                        for(size_t j=nv;j>0;--j)
                        {
                            const real_t q=d[j][k1];
                            d[j][k1] = dy[j];
                            delta    = c[j]-q;
                            dy[j]    = f1*delta;
                            c[j]     = f2*delta;
                            yz[j]   += dy[j];
                        }
                    }
                    for(size_t j=nv;j>0;--j)
                        d[j][iest]=dy[j];
                }

            }

        }

    }

}

