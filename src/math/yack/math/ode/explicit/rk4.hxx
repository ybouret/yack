
namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <>
            rk4<real_t>:: ~rk4() noexcept
            {

            }

            template <>
            rk4<real_t>:: rk4(const size_t nmax) :
            tableaux(5,nmax),
            k1( next() ),
            k2( next() ),
            k3( next() ),
            k4( next() ),
            yt( next() ),
            xadd(4)
            {}

#if !defined(YACK_RK4_CB)
#define      YACK_RK4_CB(Y,X) do { if(cb) (*cb)(Y,X); } while(false)
#endif
            template <>
            void rk4<real_t>:: operator()(writable<real_t> &y0,
                                          const real_t      x0,
                                          const real_t      x1,
                                          equation         &eq,
                                          callback         *cb)
            {
                static const real_t sixth = ( (real_t)1 )/6;
                static const real_t half  = static_cast<real_t>(0.5);
                const size_t        nvar = y0.size();

                make(nvar);
                const real_t h0 = x1-x0;
                const real_t h2 = h0 * half;
                const real_t xm = x0+h2;

                eq(k1,x0,y0); iota::muladd(yt,y0,h2,k1); YACK_RK4_CB(yt,xm);
                eq(k2,xm,yt); iota::muladd(yt,y0,h2,k2); YACK_RK4_CB(yt,xm);
                eq(k3,xm,yt); iota::muladd(yt,y0,h0,k3); YACK_RK4_CB(yt,x1);
                eq(k4,x1,yt);

                for(size_t i=nvar;i>0;--i)
                {
                    y0[i] += h0 * xadd(k1[i],twice(k2[i]),twice(k3[i]),k4[i]) * sixth;
                }
                YACK_RK4_CB(y0,x1);

            }

        }

    }
}


