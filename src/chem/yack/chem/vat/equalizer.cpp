
#include "yack/chem/vat/equalizer.hpp"

namespace yack
{
    namespace Chemical
    {


        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer()
        {
        }

        void Equalizer:: run(const xmlog      &xml,
                             writable<double> &C0,
                             const Cluster    &cl)
        {
            YACK_XMLSUB(xml,"Equalizing");

            for(const Equilibrium::Node *node=cl.standard.head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                eq.display_compact(cl.pad(std::cerr << eq.name, eq) << " : ",C0,TopLevel) << std::endl;
            }

        }
        
    }

}

