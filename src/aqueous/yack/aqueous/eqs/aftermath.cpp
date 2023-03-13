
#include "yack/aqueous/eqs/aftermath.hpp"

namespace yack
{
    namespace aqueous
    {
        aftermath:: aftermath() noexcept :
        state(is_blocked),
        value(0)
        {
        }

        aftermath:: ~aftermath() noexcept
        {
        }

        aftermath:: aftermath(const aftermath &am) noexcept :
        state(am.state),
        value(am.value)
        {
        }

        aftermath:: aftermath(const double x)      noexcept :
        state(is_running),
        value(x)
        {

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
                          triplet<double>        &x,
                          triplet<double>        &f,
                          const xlimits          &xlim,
                          cameo::mul<double>     &xmul)
        {
            const double xx = xlim.prod.xi;
            x.a = -xx; xmul = K; E.reac.mass_action(C,xx,xmul); f.a =  xmul.product();
        }


        static inline
        void make_no_reac(const equilibrium      &E,
                          const readable<double> &C,
                          triplet<double>        &x,
                          triplet<double>        &f,
                          const xlimits          &xlim,
                          cameo::mul<double>     &xmul)
        {
            xmul = 1; E.prod.mass_action(C,x.c = xlim.reac.xi,xmul); f.c = -xmul.product();
        }



        aftermath aftermath:: solve(const equilibrium      &E,
                                    const double            K,
                                    const readable<double> &C0,
                                    writable<double>       &Cs,
                                    xlimits                &xlim,
                                    cameo::mul<double>     &xmul,
                                    cameo::add<double>     &xadd)
        {

            iota::load(Cs,C0);

            triplet<double>  x = { 0 ,0,                       0};
            triplet<double>  f = { 0, E.mass_action(Cs,K,xmul),0};

            switch( __sign::of(f.c) )
            {
                case __zero__: return aftermath(0); // early return

                case positive:
                    break;

                case negative:
                    break;
            }

            const limitation l = xlim(E,Cs);
            std::cerr << E.name << " is " << xlim << std::endl;
            switch(l)
            {
                case limited_by_none: assert(0==xlim.reac.size); assert(0==xlim.prod.size);
                    throw imported::exception("aftermath","empty <%s>", E.name());

                case limited_by_both: assert(xlim.reac.size>0); assert(xlim.prod.size>0);
                    if(xlim.flag == is_blocked) return aftermath();
                    make_no_prod(E, K, Cs, x, f, xlim, xmul);
                    make_no_reac(E, Cs, x, f, xlim, xmul);
                    std::cerr << "x=" << x << std::endl;
                    std::cerr << "f=" << f << std::endl;
                    break;

                case limited_by_reac: assert(xlim.reac.size>0); assert(xlim.prod.size==0);
                    make_no_reac(E, Cs, x, f, xlim, xmul);
                    break;

                case limited_by_prod: assert(xlim.reac.size==0); assert(xlim.prod.size>0);
                    make_no_prod(E, K, Cs, x, f, xlim, xmul);
                    break;
            }

            exit(0);
            return aftermath();
        }

    }

}

