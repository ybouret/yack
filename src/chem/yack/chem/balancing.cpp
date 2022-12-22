
#include "yack/chem/balancing.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include <iomanip>


namespace yack
{
    using namespace math;

    namespace chemical
    {

        balancing:: ~balancing() throw()
        {
        }

        balancing:: balancing(const nexus &usr, const xmlog &out) :
        authority<const nexus>(usr),
        xml(out)
        {}

        bool balancing:: operator()(writable<double> &C0)
        {
            bool result = true;
            for(const cluster *cls= (**this).related.head;cls;cls=cls->next)
            {
                if(!balance(C0,*cls)) {
                    result = false;
                }
            }
            return result;
        }


        bool balancing:: balance(writable<double> &C0,
                                 const cluster    &cc)
        {

            const equilibria &eqs = (**this).lattice;

            YACK_XMLOG(xml,"balancing |cluster|=" << cc.size << " |roaming|=" << cc.roaming->size);

            for(const anode *an = (**this).working.head;an;an=an->next)
            {
                if(C0[***an]<0) goto TRY_BALANCE;
            }
            return true;

        TRY_BALANCE:
            for(const eq_node *en=cc.head;en;en=en->next)
            {
                const equilibrium &eq = **en;
                eqs.pad(std::cerr << "-> " << eq.name,eq) << " : ";
                std::cerr << feature_text(eq.kind);



                //eq.display_compact(std::cerr,C0);
                std::cerr << std::endl;
            }


            return false;
        }

    }
}

