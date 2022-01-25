

namespace yack
{
    namespace math
    {

        namespace ode
        {
            template <> rzextr<real_t>:: ~rzextr() throw() {}

            template <> rzextr<real_t>:: rzextr() : zextr<real_t>(1), fx( next() )
            {
            }


            template <>
            void rzextr<real_t>::operator()(const size_t            iest,
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

                const size_t nv = yest.size(); ensure(iest);
                
                x[iest]=xest;
                if(iest == 1)
                {
                    for(size_t j=1;j<=nv;j++)
                    {
                        yz[j]=yest[j];
                        d[j][1]=yest[j];
                        dy[j]=yest[j];
                    }
                }
                else
                {
                    for (size_t k=1;k<iest;k++)
                        fx[k+1]=x[iest-k]/xest;
                    for (size_t j=1;j<=nv;j++)
                    {
                        real_t v=d[j][1];
                        real_t c=d[j][1]=yest[j],yy=c;
                        real_t ddy = 0;
                        for(size_t k=2;k<=iest;k++)
                        {
                            real_t b1=fx[k]*v;
                            real_t b=b1-c;
                            if( std::abs(b) > 0 )
                            {
                                b=(c-v)/b;
                                ddy=c*b;
                                c=b1*b;
                            }
                            else
                            {
                                ddy=v;
                            }
                            if (k != iest) v=d[j][k];
                            d[j][k]=ddy;
                            yy += ddy;
                        }
                        dy[j]=ddy;
                        yz[j]=yy;
                    }
                }
            }

        }

    }

}

