
#include "yack/chem/eqs/aftermath.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
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
                                       const writable<Extended::Real> &Cend,
                                       const readable<Extended::Real> &Corg,
                                       Extents                        &extents,
                                       const IndexLevel                level)
        {

            const Limitation kind = extents.build(eq,Corg,level);
            Extended::Real lo = 0;
            Extended::Real hi = 0;
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
                    hi =  extents.reac;
                    lo = -extents.prod;
                    std::cerr << "lo=" << lo << " = " << *lo << std::endl;
                    std::cerr << "hi=" << hi << " = " << *hi << std::endl;
                    break;
            }



            return Aftermath();
        }
    }

}

