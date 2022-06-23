
#include "yack/chem/plexus.hpp"
namespace yack
{
    using namespace math;

    namespace chemical
    {
        void  plexus:: makeReactiveClusters()
        {

            //--------------------------------------------------------------
            // building clusters
            //--------------------------------------------------------------
            YACK_CHEM_MARKUP( vpfx, "makeReactiveClusters");

            clusters cstack;
            std::cerr << cstack << std::endl;

            
#if 0


            for(const enode *node = lattice.head(); node; node=node->next )
            {
                // get equilibrium, an start a cluster with it
                const equilibrium &eq = ***node;
                cluster           &cl = *coerce(cls).push_back( new cluster() );
                cl << &eq;

                cluster *newCl = new cluster(cl);
                delete newCl;

                YACK_CHEM_PRINTLN(vpfx << "  @" << eq.name);

                // try all other equilibria
                for(const enode *scan=node->next;scan;scan=scan->next)
                {
                    const equilibrium &it = ***scan;
                }
            }
#endif

        }
    }

}


