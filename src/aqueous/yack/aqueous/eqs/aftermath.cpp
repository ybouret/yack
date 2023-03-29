
#include "yack/aqueous/eqs/aftermath.hpp"
#include <iomanip>

namespace yack
{
    namespace aqueous
    {
        aftermath:: aftermath() noexcept :
        state(is_blocked),
        value(0),
        error(0)
        {
        }

        aftermath:: ~aftermath() noexcept
        {
        }

        aftermath:: aftermath(const aftermath &am) noexcept :
        state(am.state),
        value(am.value),
        error(am.error)
        {
        }

        aftermath:: aftermath(const double x) noexcept :
        state(is_running),
        value(x),
        error(0)
        {

        }

        aftermath:: aftermath(const double x, const double dx) noexcept :
        state(is_running),
        value(x),
        error(dx)
        {

        }

        std::ostream & operator<<(std::ostream &os, const aftermath &am)
        {
            os << xlimits::avail_text(am.state) << " @" << am.value << " => " << am.error;

            return os;
        }


    }

}

#include "yack/system/imported.hpp"
#include "yack/math/triplet.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    using namespace math;

    namespace aqueous
    {
        static inline
        void make_no_prod(const equilibrium      &E,
                          const double            K,
                          const readable<double> &C,
                          const index_level       I,
                          triplet<double>        &x,
                          triplet<double>        &f,
                          const xlimits          &xlim,
                          cameo::mul<double>     &xmul)
        {
            const double xx = xlim.prod.extent;
            x.a = -xx; xmul = K; E.reac.mass_action(I,C,xx,xmul); f.a =  xmul.product();
        }


        static inline
        void make_no_reac(const equilibrium      &E,
                          const readable<double> &C,
                          const index_level       I,
                          triplet<double>        &x,
                          triplet<double>        &f,
                          const xlimits          &xlim,
                          cameo::mul<double>     &xmul)
        {
            xmul = 1; E.prod.mass_action(I,C,x.c = xlim.reac.extent,xmul); f.c = -xmul.product();
        }

        static inline
        double am_for(const components       &E,
                      const readable<double> &C0,
                      const readable<double> &Cs,
                      const index_level       il,
                      cameo::add<double>     &xadd)
        {
            const size_t size = E->size; assert(size>0);
            xadd.free();
            for(const cnode *node = E->head; node; node=node->next)
            {
                const component &cc = ***node;
                const size_t     i  = (*cc).indx[il];
                xadd.push( (Cs[i]-C0[i]) / cc.nu );
            }
            assert(size==xadd.size());
            return xadd.sum()/size;
        }


        aftermath aftermath:: solve(const equilibrium      &E,
                                    const double            K,
                                    const readable<double> &C0,
                                    writable<double>       &Cs,
                                    const index_level       I,
                                    xlimits                &xlim,
                                    cameo::mul<double>     &xmul,
                                    cameo::add<double>     &xadd)
        {

            iota::load(Cs,C0);


            //------------------------------------------------------------------
            //
            // initializing limit and local triplets
            //
            //------------------------------------------------------------------
        CYCLE:
            triplet<double>  x = { 0 ,0, 0};
            triplet<double>  f = { 0, 0, 0};
            const limitation l = xlim(I,E,Cs);

            switch(l)
            {
                    //----------------------------------------------------------
                    //
                    // no limitation : bad
                    //
                    //----------------------------------------------------------
                case limited_by_none: assert(0==xlim.reac.size); assert(0==xlim.prod.size);
                    throw imported::exception("aftermath","empty <%s>", E.name());

                    //----------------------------------------------------------
                    //
                    // both limits: check w.r.t current mass action
                    //
                    //----------------------------------------------------------
                case limited_by_both: assert(xlim.reac.size>0); assert(xlim.prod.size>0);
                    if(xlim.flag == is_blocked) return aftermath();
                    switch( __sign::of(f.b=E.mass_action(I,Cs,K,xmul)) )
                    {
                        case __zero__: return aftermath( am_for(E,C0,Cs,I,xadd) );                             // early return
                        case positive: f.a = f.b; make_no_reac(E,Cs,I,x,f,xlim,xmul);   assert(f.c<0);  break; // in [0:x.c>0]
                        case negative: f.c = f.b; make_no_prod(E,K,Cs,I,x,f,xlim,xmul); assert(f.a>0);  break; // in [x.a<0:0]
                    }
                    break;

                    //----------------------------------------------------------
                    //
                    // reactants only: check w.r.t mass action
                    //
                    //----------------------------------------------------------
                case limited_by_reac: assert(xlim.reac.size>0); assert(xlim.prod.size==0);
                    switch( __sign::of(f.b=E.mass_action(I,Cs,K,xmul)) )
                    {
                        case __zero__: return aftermath( am_for(E,C0,Cs,I,xadd) );    // early return
                        case positive: f.a = f.b; x.c=xlim.reac.extent; f.c=-1; break; // in [0:x.c>0]
                        case negative:
                            f.c = f.b;
                            assert(E.d_nu<0);
                            assert(E.idnu<0);
                            x.a  = -pow(K,E.idnu);
                            while( (f.a = E.mass_action(I,Cs,K,x.a,xmul) ) <= 0)
                                x.a += x.a;
                            break;
                    }

                    break;

                    //----------------------------------------------------------
                    //
                    // poroducts only: check w.r.t mass action
                    //
                    //----------------------------------------------------------
                case limited_by_prod: assert(xlim.reac.size==0); assert(xlim.prod.size>0);
                    switch( __sign::of(f.b=E.mass_action(I,Cs,K,xmul)) )
                    {
                        case __zero__: return aftermath( am_for(E,C0,Cs,I,xadd) );                               // early return
                        case negative: f.c = f.b; make_no_prod(E,K,Cs,I,x,f,xlim,xmul); assert(f.a>0); break; // in [x.a<0:0]
                        case positive: f.a = f.b;
                            assert(E.d_nu>0);
                            assert(E.idnu>0);
                            x.c  = pow(K,E.idnu);
                            while( (f.c = E.mass_action(I,Cs,K,x.c,xmul) ) >= 0)
                                x.c += x.c;
                            assert(x.c>0);
                            assert(f.c<0);
                            break;
                    }
                    break;
            }



            
            double width = fabs(x.c-x.a);
        BISECT:
            assert(f.a>0);
            assert(f.c<0);
            f.b = E.mass_action(I,Cs,K,x.b=clamp(x.a,0.5*(x.a+x.c),x.c),xmul);
            switch( __sign::of(f.b) )
            {
                case __zero__:  E.move(Cs,x.b); return aftermath( am_for(E,C0,Cs,I,xadd) ); // early return
                case positive:  f.a = f.b; x.a = x.b; break;
                case negative:  f.c = f.b; x.c = x.b; break;
            }
            const double new_width = fabs(x.c-x.a);
            if(new_width<width)
            {
                width = new_width;
                goto BISECT;
            }


            bool converged = true;
            for(const cnode *node=E->head;node;node=node->next)
            {
                const component &cm = ***node;                 // compnent
                const size_t     ii = (*cm).indx[0];           // index species
                const double     c0 = Cs[ii];                  // starting point
                const double     c1 = (Cs[ii] += cm.nu * x.b); // final point
                if(fabs(c0-c1)>0)
                {
                    converged = false;
                }
            }

            if(!converged) goto CYCLE;

            // converged with numerical error
            return aftermath( am_for(E,C0,Cs,I,xadd), E.mass_action(I,Cs,K,xmul) );
        }

    }

}

