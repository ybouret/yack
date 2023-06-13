
#include "yack/chem/eqs/aftermath.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {

        Aftermath:: ~Aftermath() noexcept
        {
        }

        Aftermath:: Aftermath() noexcept : extent(0), status(Equilibrium::Blocked), action(0)
        {
        }
        
        Aftermath:: Aftermath(const Extended::Real args) noexcept : extent(args), status(Equilibrium::Running), action(0)
        {
        }

        Aftermath:: Aftermath(const Extended::Real args,
                              const Extended::Real aerr) noexcept : extent(args), status(Equilibrium::Running), action(aerr)
        {
        }

        Aftermath:: Aftermath(const Aftermath &am) noexcept :
        extent(am.status), status(am.status), action(am.action)
        {

        }


        std::ostream & operator<<(std::ostream &os, const Aftermath &am)
        {
            os << Equilibrium::StatusText(am.status) << " @" << std::setw(15) << *am.extent << " (action=" << am.action << ")";
            return os;
        }

        bool Aftermath:: QueryLimitedByBoth(Extended::Triplet              &xi,
                                            Extended::Triplet              &ma,
                                            const Equilibrium              &eq,
                                            const Extended::Real           &K,
                                            writable<Extended::Real>       &Ctmp,
                                            const readable<Extended::Real> &Cend,
                                            const Extents                  &extents,
                                            const IndexLevel                level,
                                            Extended::Mul                  &xmul)
        {
            xi.b = xmul._0;
            ma.b = eq.massAction(xmul,K,Cend,level);
            switch( __sign::of(ma.b.m) )
            {

                    //----------------------------------------------------------
                case __zero__: // found already running and solved
                    //----------------------------------------------------------
                    return true;

                    //----------------------------------------------------------
                case negative: // move prod
                    //            to get a positive mass action from reac only
                    //----------------------------------------------------------
                    assert(extents.prod>0);
                    xi.a = -extents.prod;
                    eq.make(Ctmp, level, Cend, level, xi.a);
                    extents.prod.nullify(Ctmp,level);
                    ma.a = eq.reacMassAction(xmul,K,Ctmp,level);
                    xi.c = xi.b;
                    ma.c = ma.b;
                    break;

                    //----------------------------------------------------------
                case positive: // move reac
                    //            to get a negative mass action from prod only
                    //----------------------------------------------------------
                    assert(extents.reac>0);
                    xi.c =  extents.reac;
                    eq.make(Ctmp, level, Cend, level, xi.c);
                    extents.reac.nullify(Ctmp,level);
                    ma.c = -eq.prodMassAction(xmul,Ctmp,level);
                    xi.a = xi.b;
                    ma.a = ma.b;
            }

            assert(xi.c>xi.a);
            assert(ma.c<ma.a);

            return false;
        }


        Extended::Real Aftermath:: makeExtent(const Components               &eq,
                                              const readable<Extended::Real> &Corg,
                                              const readable<Extended::Real> &Cend,
                                              const IndexLevel                level,
                                              Extended::Add                  &xadd)
        {
            xadd.free();
            assert(eq->size>0);
            for(const cNode *node=eq->head;node;node=node->next)
            {
                const Species &s = ****node;
                const size_t   j = s.indx[level];
                const Extended::Real del = Cend[j]-Corg[j];
                xadd.append(del);

            }
            return xadd.reduce()/eq->size;
        }

        Aftermath Aftermath:: Evaluate(const Equilibrium              &eq,
                                       const Extended::Real           &K,
                                       writable<Extended::Real>       &Cend,
                                       const readable<Extended::Real> &Corg,
                                       Extents                        &extents,
                                       const IndexLevel                level,
                                       Extended::Mul                  &xmul,
                                       Extended::Add                  &xadd,
                                       writable<Extended::Real>       &Ctmp)
        {
            const Extended::Real _0   = xmul._0;
            Extended::Triplet    xi;
            Extended::Triplet    ma;
            keto::load(Cend,Corg);

            Extended::Real absXi = 0;       // must decrease
            bool           check = false;   // but not the first time
        LOOKUP:
            switch( extents.build(eq,Cend,level) )
            {
                case LimitedByNone: throw imported::exception(eq.name(),"empty equilibrium");

                case LimitedByProd:
                    return Aftermath();
                    break;

                case LimitedByReac:
                    return Aftermath();
                    break;

                case LimitedByBoth:
                    // check blocked or not
                    if(extents.reac.isBlocking() && extents.prod.isBlocking())
                    {
                        return Aftermath();
                    }

                    // check if 0 is solution for Cend, otherwise init
                    if( QueryLimitedByBoth(xi,ma,eq,K,Ctmp,Cend,extents,level,xmul))
                    {
                        return Aftermath(xmul._0);
                    }
                    break; // => bisection
            }

            //std::cerr << "---- BISECTION ----" << std::endl;
            Extended::Real width = xi.c - xi.a;
            while(true)
            {
                xi.b = (xi.a+xi.c).half();
                eq.make(Ctmp, level, Cend, level, xi.b);
                ma.b = eq.massAction(xmul,K,Ctmp,level);
                //std::cerr << "\txi: " << xi << std::endl;
                //std::cerr << "\tma: " << ma << std::endl;
                //std::cerr << "\t w: " << width << std::endl;
                switch( __sign::of(ma.b) )
                {
                    case __zero__:
                        keto::load(Cend,Ctmp);
                        xi.b = makeExtent(eq,Corg,Cend,level,xadd);
                        //std::cerr << "EXACT MA=0 @XI=" << xi.b << std::endl;
                        //eq.displayCompact(std::cerr << "\t" << eq << " exact: ",Cend,level) << " => " << eq.massAction(xmul,K,Cend,level) << std::endl;
                        return Aftermath( xi.b );

                    case negative:
                        xi.c = xi.b;
                        ma.c = ma.b;
                        break;

                    case positive:
                        xi.a = xi.b;
                        ma.a = ma.b;
                        break;
                }
                const Extended::Real newWidth = xi.c - xi.a;
                if(newWidth>=width)
                {
                    break;
                }
                width = newWidth;
            }

            const Extended::Real absTmp = xi.b.abs();
            keto::load(Cend,Ctmp);
            if(check && absTmp>=absXi)
            {
                //std::cerr << "Saturation" << std::endl;
                xi.b = makeExtent(eq,Corg,Cend,level,xadd);
                //eq.displayCompact(std::cerr << "\t" << eq << " approx: ",Cend,level)
                //<< " => " <<ma.b << std::endl;
                return Aftermath(xi.b,ma.b);
            }

            absXi = absTmp;
            check = true;
            goto LOOKUP;


        }
    }

}

