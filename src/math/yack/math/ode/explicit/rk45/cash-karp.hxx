

namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <> cash_karp<real_t>:: ~cash_karp() noexcept {}

            template <> cash_karp<real_t>::  cash_karp() :
            base_type(6),
            ak2( next() ),
            ak3( next() ),
            ak4( next() ),
            ak5( next() ),
            ak6( next() ),
            ytmp( next() ),
            xadd(6)
            {
                
            }

            template <>
            void cash_karp<real_t>:: operator()(const readable<real_t> &y,
                                                const readable<real_t> &dydx,
                                                const real_t            x,
                                                const real_t            h,
                                                writable<real_t>       &yout,
                                                writable<real_t>       &yerr,
                                                equation               &drvs,
                                                callback               *proc)
            {
                static const real_t a2=0.2,a3=0.3,a4=0.6,a5=1.0,a6=0.875;
                static const real_t b21=0.2,
                b31=3.0/40.0,b32=9.0/40.0,b41=0.3,b42 = -0.9,b43=1.2,
                b51 = -11.0/54.0, b52=2.5,b53 = -70.0/27.0,b54=35.0/27.0,
                b61=1631.0/55296.0,b62=175.0/512.0,b63=575.0/13824.0,
                b64=44275.0/110592.0,b65=253.0/4096.0,c1=37.0/378.0,
                c3=250.0/621.0,c4=125.0/594.0,c6=512.0/1771.0,
                dc5 = -277.00/14336.0;
                static const real_t dc1=c1-2825.0/27648.0,dc3=c3-18575.0/48384.0,
                dc4=c4-13525.0/55296.0,dc6=c6-0.25;

                assert( y.size() == dydx.size() );
                assert( y.size() == yout.size() );
                assert( y.size() == yerr.size() );
                const size_t n = y.size();
                make(n);
                
                {
                    const real_t fac = b21*h;
                    const real_t xx  = x+a2*h;
                    for(size_t i=n;i>0;--i) ytmp[i]=y[i]+fac*dydx[i];
                    if(proc) (*proc)(ytmp,xx);
                    drvs(ak2,xx,ytmp);
                }

                {
                    const real_t xx = x+a3*h;
                    for(size_t i=n;i>0;--i) ytmp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);
                    if(proc) (*proc)(ytmp,xx);
                    drvs(ak3,xx,ytmp);
                }

                {
                    const real_t xx = x+a4*h;
                    //for(size_t i=n;i>0;--i) ytmp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
                    for(size_t i=n;i>0;--i) ytmp[i]=y[i]+h*xadd(b41*dydx[i],b42*ak2[i],b43*ak3[i]);
                    if(proc) (*proc)(ytmp,xx);
                    drvs(ak4,xx,ytmp);
                }

                {
                    const real_t xx = x+a5*h;
                    //for(size_t i=n;i>0;--i) ytmp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);
                    for(size_t i=n;i>0;--i) ytmp[i]=y[i]+h*xadd(b51*dydx[i],b52*ak2[i],b53*ak3[i],b54*ak4[i]);
                    if(proc) (*proc)(ytmp,xx);
                    drvs(ak5,xx,ytmp);
                }

                {
                    const real_t xx = x+a6*h;
                    //for(size_t i=n;i>0;--i) ytmp[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);
                    for(size_t i=n;i>0;--i) ytmp[i]=y[i]+h*xadd(b61*dydx[i],b62*ak2[i],b63*ak3[i],b64*ak4[i],b65*ak5[i]);
                    if(proc) (*proc)(ytmp,xx);
                    drvs(ak6,xx,ytmp);
                }

                // evaluate output
                //for(size_t i=n;i>0;--i) yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c6*ak6[i]);
                for(size_t i=n;i>0;--i) yout[i]=y[i]+h*xadd(c1*dydx[i],c3*ak3[i],c4*ak4[i],c6*ak6[i]);
                if(proc) (*proc)(yout,x+h);

                // evaluate error
                //for(size_t i=n;i>0;--i) yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]);
                for(size_t i=n;i>0;--i) yerr[i]=h*xadd(dc1*dydx[i],dc3*ak3[i],dc4*ak4[i],dc5*ak5[i],dc6*ak6[i]);


            }


        }
    }

}
