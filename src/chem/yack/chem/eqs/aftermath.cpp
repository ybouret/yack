
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

        Aftermath Aftermath:: Evaluate(const Equilibrium              &eq,
                                       const Extended::Real           &K,
                                       writable<Extended::Real>       &Cend,
                                       const readable<Extended::Real> &Corg,
                                       Extents                        &extents,
                                       const IndexLevel                level,
                                       Extended::Mul                  &xmul)
        {

            const Limitation  kind = extents.build(eq,Corg,level);
            Extended::Triplet xi;
            Extended::Triplet ma;

            keto::load(Cend,Corg);

            switch( kind )
            {
                case LimitedByNone: throw imported::exception(eq.name(),"empty equilibrium");

                case LimitedByProd:
                    break;

                case LimitedByReac:
                    break;

                case LimitedByBoth:
                    if(extents.reac.isBlocking() && extents.prod.isBlocking())
                        return Aftermath();
                    xi.a = -extents.prod;
                    eq.make(Cend, level, Corg, level, xi.a);
                    extents.prod.nullify(Cend,level);
                    ma.a = eq.reacMassAction(xmul,K,Cend,level);
                    eq.displayCompact(std::cerr << "\t" << eq << " noProd: ",Cend,level) << " => ma = " << ma.a <<  " / " << eq.massAction(xmul,K,Cend,level) << std::endl;

                    xi.c =  extents.reac;
                    eq.make(Cend, level, Corg, level, xi.c);
                    extents.reac.nullify(Cend,level);
                    ma.c = -eq.prodMassAction(xmul,Cend,level);
                    eq.displayCompact(std::cerr << "\t" << eq << " noReac: ",Cend,level) << " => ma = " << ma.c << " / " << eq.massAction(xmul,K,Cend,level) << std::endl;

                    assert(xi.c>xi.a);
                    assert(ma.c<ma.a);

                    break;
            }

            






            return Aftermath();
        }
    }

}

