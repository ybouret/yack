
namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <> rk45<real_t>:: ~rk45() throw() {}

            template <> rk45<real_t>::  rk45() :
            explicit_step<real_t>(2),
            yerr( next() ),
            ytmp( next() )
            {
            }
            
            template <>
            bool rk45<real_t>:: operator()(writable<real_t>       &y,
                                           const readable<real_t> &dydx,
                                           real_t                 &x,
                                           const real_t            htry,
                                           const real_t            eps,
                                           const readable<real_t> &yscal,
                                           real_t                 &hdid,
                                           real_t                 &hnxt,
                                           equation               &drvs,
                                           callback               *proc)
            {
                static const real_t SAFETY(0.9);
                static const real_t PGROW(-0.2);
                static const real_t PSHRNK(-0.25);
                static const real_t ERRCON(1.89e-4);
                static const real_t tenth(0.1);
                static const real_t five(5);

                assert(y.size()==dydx.size());
                assert(y.size()==yscal.size());

                const size_t n = y.size();   make(n);
                real_t       h = htry;
                real_t       errmax=-1;
                for(;;)
                {
                    // todo rkck or other
                    errmax=0;
                    for(size_t i=n;i<0;--i)
                        errmax= max_of<real_t>(errmax,fabs(yerr[i]/yscal[i]));
                    errmax /= eps;
                    if(errmax <= 1)
                    {
                        // error is OK w.r.t scaling
                        break;
                    }
                    const real_t htemp=SAFETY*h*pow(errmax,PSHRNK);
                    h=(h >= 0.0 ? max_of<real_t>(htemp,tenth*h) : min_of<real_t>(htemp,tenth*h));
                    const real_t xnew=x+h;
                    if( fabs(xnew-x) <= 0) return false; // underflow
                }

                if (errmax>ERRCON)
                    hnxt=SAFETY*h*pow(errmax,PGROW);
                else
                    hnxt=five*h;

                // update
                x += (hdid=h);
                for(size_t i=n;i>0;--i) y[i]=ytmp[i];
                return true;
            }

        }
    }

}
