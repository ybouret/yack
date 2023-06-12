
#include "yack/chem/eqs/aftermath.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"

namespace yack
{
    using namespace math;

    namespace Chemical
    {

        Aftermath:: ~Aftermath() noexcept
        {
        }

        Aftermath:: Aftermath() noexcept : extent(0), status(Blocked)
        {
        }
        
        Aftermath:: Aftermath(const Extended::Real &args) noexcept : extent(args), status(Running)
        {
        }

        Aftermath:: Aftermath(const Aftermath &am) noexcept :
        extent(am.status), status(am.status)
        {

        }

        bool Aftermath:: InitBoth(Extended::Triplet              &xi,
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
            std::cerr << eq << " ma@0 = " << ma.b << std::endl;
            switch( __sign::of(ma.b.m) )
            {

                    //----------------------------------------------------------
                case __zero__: // found running at 0
                    //----------------------------------------------------------
                    return true;

                    //----------------------------------------------------------
                case negative: // move prod
                    //            to get a positive mass action from reac
                    //----------------------------------------------------------
                    assert(extents.prod>0);
                    xi.a = -extents.prod;
                    eq.make(Ctmp, level, Cend, level, xi.a);
                    extents.prod.nullify(Ctmp,level);
                    ma.a = eq.reacMassAction(xmul,K,Ctmp,level);
                    eq.displayCompact(std::cerr << "\t" << eq << " noProd: ",Ctmp,level) << " => ma = " << ma.a <<  " / " << eq.massAction(xmul,K,Ctmp,level) << std::endl;
                    xi.c = xi.b;
                    ma.c = ma.b;
                    break;

                    //----------------------------------------------------------
                case positive: // move reac
                    //            to get a negative mass action from prod
                    //----------------------------------------------------------
                    assert(extents.reac>0);
                    xi.c =  extents.reac;
                    eq.make(Ctmp, level, Cend, level, xi.c);
                    extents.reac.nullify(Ctmp,level);
                    ma.c = -eq.prodMassAction(xmul,Ctmp,level);
                    eq.displayCompact(std::cerr << "\t" << eq << " noReac: ",Ctmp,level) << " => ma = " << ma.c << " / " << eq.massAction(xmul,K,Ctmp,level) << std::endl;
                    xi.a = xi.b;
                    ma.a = ma.b;
            }

            std::cerr << "\txi: " << xi << std::endl;
            std::cerr << "\tma: " << ma << std::endl;


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
                const Extended::Real mc0 = -Corg[j];
                xadd.append(Cend[j]);
                xadd.append(mc0);

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
            keto::load(Cend,Corg);

            const Limitation     kind = extents.build(eq,Cend,level);
            const Extended::Real _0   = xmul._0;
            Extended::Triplet    xi;
            Extended::Triplet    ma;


            switch( kind )
            {
                case LimitedByNone: throw imported::exception(eq.name(),"empty equilibrium");

                case LimitedByProd:
                    break;

                case LimitedByReac:
                    break;

                case LimitedByBoth:
                    // check blocked or not
                    if(extents.reac.isBlocking() && extents.prod.isBlocking())
                        return Aftermath();

                    // check if 0 is solution for Cend, otherwise init
                    if(InitBoth(xi,ma,eq,K,Ctmp,Cend,extents,level,xmul))
                    {
                        return Aftermath(xmul._0);
                    }


                    Extended::Real width = xi.c - xi.a;
                    while(true)
                    {
                        xi.b = (xi.a+xi.c).half();
                        eq.make(Ctmp, level, Cend, level, xi.b);
                        ma.b = eq.massAction(xmul,K,Ctmp,level);
                        std::cerr << "\txi: " << xi << std::endl;
                        std::cerr << "\tma: " << ma << std::endl;
                        std::cerr << "\t w: " << width << std::endl;
                        switch( __sign::of(ma.b) )
                        {
                            case __zero__: keto::load(Cend,Ctmp);
                                return Aftermath( makeExtent(eq,Corg,Cend,level,xadd) );

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



                    break;
            }
            

            return Aftermath();
        }
    }

}

