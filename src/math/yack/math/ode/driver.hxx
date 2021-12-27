
namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <> driver<real_t>:: ~driver() throw() {}
            template <> driver<real_t>::  driver() :
            tableaux(3,0),
            nfit(0),
            nbad(0),
            tiny(1e-30),
            y(     next() ),
            dydx(  next() ),
            yscal( next() )
            {}


            template <>
            void driver<real_t>:: operator()(step<real_t>     &moving,
                                             writable<real_t> &ystart,
                                             const real_t      x1,
                                             const real_t      x2,
                                             const real_t      eps,
                                             real_t           &h1,
                                             equation          drvs,
                                             callback         *proc)
            {
                const size_t n = ystart.size();
                make(n);  
                nfit=0;
                nbad=0;

                real_t x = x1;
                real_t h = __sgn(h1,x2-x1);
                for(size_t i=n;i>0;--i) y[i] = ystart[i];

                while(true)
                {
                    //__________________________________________________________
                    //
                    // initialize derivatives@x
                    //__________________________________________________________
                    drvs(dydx,x,y);

                    //__________________________________________________________
                    //
                    // check that next step moves and does not overflow
                    //__________________________________________________________
                    bool would_finish = false;
                    {
                        const real_t xp = x+h;
                        if ((xp-x2)*(xp-x1) >= 0) // xp out of interval
                        {
                            h=x2-x;
                            would_finish = true;
                        }
                    }

                    //__________________________________________________________
                    //
                    // estimate scaling
                    //__________________________________________________________
                    for(size_t i=n;i>0;--i)
                    {
                        yscal[i] = fabs(y[i]) + fabs(h*dydx[i]) + tiny;
                    }

                    //__________________________________________________________
                    //
                    // take step
                    //__________________________________________________________
                    real_t       hdid=0;
                    real_t       hnxt=0;
                    moving(y,dydx,x,h,eps,yscal,hdid,hnxt,drvs,proc);


                    //__________________________________________________________
                    //
                    // check what was done
                    //__________________________________________________________
                    if(fabs(h-hdid)<=0)
                    {
                        //______________________________________________________
                        //
                        // took required step
                        //______________________________________________________
                        ++nfit;
                        if(would_finish)
                        {
                            for(size_t i=n;i>0;--i) ystart[i] = y[i];
                            h1 = hnxt;
                            return;
                        }
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // decreased
                        //______________________________________________________
                        ++nbad;
                    }


                    //__________________________________________________________
                    //
                    // take next step
                    //__________________________________________________________
                    h = hnxt;
                }



            }
        }
    }

}
