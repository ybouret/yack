
namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <> dormand_prince<real_t>:: ~dormand_prince() noexcept {}

            template <> dormand_prince<real_t>::  dormand_prince() :
            base_type(7),
            ak2( next()  ),
            ak3( next()  ),
            ak4( next()  ),
            ak5( next()  ),
            ak6( next()  ),
            ak7( next()  ),
            ytmp( next() )
            {

            }

            template <>
            void dormand_prince<real_t>:: operator()(const readable<real_t> &y,
                                                     const readable<real_t> &dydx,
                                                     const real_t            x,
                                                     const real_t            h,
                                                     writable<real_t>       &yout,
                                                     writable<real_t>       &yerr,
                                                     equation               &drvs,
                                                     callback               *proc)
            {
                static const real_t a2  = REAL(1.0)/REAL(5.0), a3 = REAL(3.0)/REAL(10.0), a4 = REAL(4.0)/REAL(5.0), a5 = REAL(8.0)/REAL(9.0);// a6=1.0;
                static const real_t b21 = REAL(1.0)/REAL(5.0);
                static const real_t b31 = REAL(3.0)/REAL(40.0),       b32 = REAL(9.0)/REAL(40.0);
                static const real_t b41 = REAL(44.0)/REAL(45.0),      b42 = REAL(-56.0)/REAL(15.0),      b43 = REAL(32.0)/REAL(9.0);
                static const real_t b51 = REAL(19372.0)/REAL(6561.0), b52 =    REAL(-25360.0)/REAL(2187.0), b53 = REAL(64448.0)/REAL(6561.0),        b54 = REAL(-212.0)/REAL(729.0);
                static const real_t b61 = REAL(9017.0)/REAL(3168.0),  b62 = REAL(-355.0)/REAL(33.0),     b63 = REAL(46732.0)/REAL(5247.0),      b64 = REAL(49.0)/REAL(176.0),     b65 = REAL(-5103.0)/REAL(18656.0);
                static const real_t c1  = REAL(35.0)/REAL(384.0),                            c3  = REAL(500.0)/REAL(1113.0),        c4  = REAL(125.0)/REAL(192.0),    c5  = REAL(-2187.0)/REAL(6784.0),       c6=REAL(11.0)/REAL(84.0);
                static const real_t dc1 = c1 - REAL(5179.0)/REAL(57600.0),                     dc3 = c3 -    REAL(7571.0)/REAL(16695.0),    dc4 = c4-REAL(393.0)/REAL(640.0), dc5 = c5+REAL(92097.0)/REAL(339200.0),  dc6 = c6 - REAL(187.0)/REAL(2100.0), dc7 =  REAL(-1.0)/REAL(40.0);

                const size_t n = y.size(); make(n);

                //______________________________________________________________
                //
                //assuming y is valid w.r.t the callback
                //______________________________________________________________
#define RKDP_CHK(A) xx=x+A*h; if (proc) (*proc)(ytmp,xx)

                {
                    const real_t fac = b21*h;
                    for (size_t i=n;i>0;--i)
                    {
                        ytmp[i]=y[i]+fac*dydx[i];
                    }
                }


                real_t RKDP_CHK(a2);
                drvs(ak2,xx,ytmp);
                for (size_t i=n;i>0;--i)
                {
                    ytmp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);
                }


                
                RKDP_CHK(a3);
                drvs(ak3,xx,ytmp);

                for (size_t i=n;i>0;--i)
                {
                    ytmp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
                }

                RKDP_CHK(a4);
                drvs(ak4,xx,ytmp);

                for (size_t i=n;i>0;--i)
                {
                    ytmp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);
                }

                RKDP_CHK(a5);
                drvs(ak5,xx,ytmp);

                for (size_t i=n;i>0;--i)
                {
                    ytmp[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);
                }

                RKDP_CHK(1);
                drvs(ak6,xx,ytmp);

                for (size_t i=n;i>0;--i)
                {
                    yout[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c5*ak5[i]+c6*ak6[i]);
                }

                if(proc) (*proc)(yout,xx);

                drvs(ak7,xx,yout); // ak7 is new dxdy at end of step

                for (size_t i=n;i>0;--i)
                {
                    yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]+dc7*ak7[i]);
                }
            }

        }
    }
}

