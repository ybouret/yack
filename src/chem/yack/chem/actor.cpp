
#include "yack/chem/actor.hpp"
#include "yack/arith/ipower.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace chemical
    {

        actor:: ~actor() throw()
        {
        }

        actor:: actor(const component &c) throw() :
        object(),
        authority<const species>(*c),
        nu( absolute(c.nu) ),
        nu1(nu-1),
        next(0),
        prev(0)
        {
        }


        double actor:: extent(const readable<double> &C) const throw()
        {
            assert( (**this)(C) >= 0);
            return (**this)(C)/nu;
        }

        double actor:: action(const readable<double> &C) const throw()
        {
            assert( (**this)(C) >= 0);
            return ipower( (**this)(C), nu );
        }


        double actor:: derivs(double K, const readable<double> &C) const throw()
        {
            assert( (**this)(C) >= 0);
            switch (nu)
            {
                case 0: return 0;                         //!< shouldn't happen
                case 1: break;                            //!< common
                case 2: K += K; K *= (**this)(C); break;  //!< common
                default:
                    assert(nu>=3);
                    K *= nu;
                    K *= ipower( (**this)(C), nu1);
                    break;
            }
            return K;
        }


    }
}
