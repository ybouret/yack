namespace yack
{
    namespace math
    {

        namespace ode
        {
            template <> midpoint<real_t>:: ~midpoint() throw() {}
            template <> midpoint<real_t>::  midpoint() : tableaux(2,0),
            yn( next() ),
            ym( next() )
            {

            }

            template <>
            void midpoint<real_t>:: operator()(const readable<real_t> &y,
                                               const readable<real_t> &dydx,
                                               const real_t            xs,
                                               const real_t            htot,
                                               const size_t            nstep,
                                               writable<real_t>       &yout,
                                               equation               &drvs,
                                               callback               *proc)
            {
                assert( y.size() == dydx.size() );
                assert( y.size() == yout.size() );
                static const real_t half(0.5);
                const size_t        nvar = y.size(); make(nvar);
                const real_t        h    = htot/nstep;
                for(size_t i=nvar;i>0;--i)
                {
                    ym[i]=y[i];
                    yn[i]=y[i]+h*dydx[i];
                }
                real_t x=xs+h;
                if(proc) (*proc)(yn,x);
                drvs(yout,x,yn);
                const real_t h2 = 2*h;
                for(size_t n=2;n<=nstep;n++)
                {
                    for(size_t i=nvar;i>0;--i)
                    {
                        const real_t swap=ym[i]+h2*yout[i];
                        ym[i]=yn[i];
                        yn[i]=swap;
                    }
                    if(proc) (*proc)(yn,x);
                    x += h;
                    drvs(yout,x,yn);
                }
                for(size_t i=nvar;i>0;--i)
                {
                    yout[i]=half*(ym[i]+yn[i]+h*yout[i]);
                }
                if(proc) (*proc)(yout,x);
            }
        }
        
    }
}
