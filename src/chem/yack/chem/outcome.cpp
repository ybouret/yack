
#include "yack/chem/outcome.hpp"

namespace yack
{
    namespace chemical
    {
        outcome:: outcome(const components::state s,
                          const extent::grade     g,
                          const double            x) throw() :
        state(s),
        grade(g),
        value(x)
        {

        }

        outcome:: ~outcome() throw()
        {
        }

        outcome:: outcome(const outcome &other) throw() :
        state(other.state),
        grade(other.grade),
        value(other.value)
        {
        }
        
    }

}

#include "yack/math/iota.hpp"
#include "yack/math/triplet.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/ocstream.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        struct MassActionF
        {
            const components       &comp;
            const double            K;
            const readable<double> &C;
            rmulops                &ops;

            double operator()(const double xx)
            {
                return comp.mass_action(K,C,xx,ops);
            }
        };

        enum search_direction
        {
            search_left,
            search_right
        };

        outcome outcome:: study(const components       &comp,
                                const double            K,
                                const readable<double> &Cini,
                                writable<double>       &Cend,
                                rmulops                &ops)
        {

            assert(K>0);

            // initialize
            iota::save(Cend,Cini);
            switch( comp.state_at(Cini) )
            {
                case components::are_blocked: return outcome(components::are_blocked,extent::is_degenerated,0);
                case components::are_running: break;
            }
            triplet<double>  f  = { 0,comp.mass_action(K,Cini,ops),0 };
            const sign_type  s  = __sign::of(f.b);
            search_direction d  = search_left;
            std::cerr << "f.b=" << f.b << std::endl;
            switch(s)
            {
                case __zero__: return outcome(components::are_running,extent::is_degenerated,0);
                case positive:
                    d = search_right;
                    std::cerr << "search right" << std::endl;
                    break;
                case negative:
                    std::cerr << "search left" << std::endl;
                    break;
            }

            triplet<double>  x  = { 0,0,0 };
            MassActionF      F  = { comp, K, Cend, ops };
            std::cerr << " -- K=" << K << " --" << std::endl;
            {
                // study limits
                const xlimits &lm = comp.genuine_limits(Cini,0);
                std::cerr << "limits=" << lm << std::endl;

                switch(lm.type)
                {

                    case limited_by_none:
                        return outcome(components::are_running,extent::is_degenerated,0);

                    case limited_by_reac:
                        assert(0==comp.prod.molecularity);
                        switch(d)
                        {
                            case search_right:
                                f.a = f.b; assert(f.a>0);
                                x.a = 0;
                                x.c = lm.reac->xi;
                                ops.ld1();
                                f.c = -1;
                                assert(f.c<0);
                                break;

                            case search_left:
                                f.c = f.b; assert(f.c<0);
                                x.c = 0;
                                x.a = -pow(K,-1.0/comp.reac.molecularity);
                                //std::cerr << "scaling=" << x.a << std::endl;
                                while( (f.a = F(x.a) ) <= 0 ) x.a += x.a;
                                break;
                        }
                        break;

                    case limited_by_prod:
                        assert(0==comp.reac.molecularity);
                        switch(d)
                        {
                            case search_left:
                                f.c = f.b; assert(f.c<0);
                                x.c = 0;
                                x.a = -lm.prod->xi;
                                f.a = K;
                                assert(f.a>0);
                                break;

                            case search_right:
                                f.a = f.b; assert(f.a>0);
                                x.a = 0;
                                x.c = pow(K,1.0/comp.prod.molecularity);
                                //std::cerr << "scaling=" << x.c << std::endl;
                                while( (f.c = F(x.c) ) >= 0 ) x.c += x.c;
                                break;
                        }
                        break;

                    case limited_by_both:
                        switch(d)
                        {
                            case search_left:
                                f.c = f.b; assert(f.c<0);
                                x.c = 0;
                                x.a = -lm.prod->xi;
                                ops = K;
                                f.a = comp.reac.mass_action(Cini,lm.prod->xi,ops);
                                assert(f.a>0);
                                break;

                            case search_right:
                                f.a = f.b; assert(f.a>0);
                                x.a = 0;
                                x.c = lm.reac->xi;
                                ops.ld1();
                                f.c = -comp.prod.mass_action(Cini,x.c,ops);
                                assert(f.c<0);
                                break;
                        }
                        break;
                }
            }

            std::cerr << "x=" << x << ", f=" << f << std::endl;

            if( __sign::product( __sign::of(f.a), __sign::of(f.c) ) != negative )
            {
                throw exception("CORRUPTED");
            }

            if(true)
            {
                ios::ocstream fp("out.dat");
                const size_t np=100;
                for(size_t i=0;i<=np;++i)
                {
                    const double xx = x.a + (i*(x.c-x.a))/np;
                    fp("%g %g\n",xx, F(xx) );
                }

            }



            return outcome(components::are_running,extent::is_degenerated,0);
        }

    }

}



