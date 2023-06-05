#include "yack/chem/vat/equalizer.hpp"

namespace yack
{
    namespace Chemical
    {


        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer() :
        speciesFund( new Species::Bank() ),
        cursorsFund( new Cursor::Bank()  ),
        reac( speciesFund, cursorsFund   ),
        prod( speciesFund, cursorsFund   ),
        xadd()
        {
        }


        
        void Equalizer:: run(const xmlog      &xml,
                             writable<double> &C0,
                             const Cluster    &cl)
        {
            YACK_XMLSUB(xml,"Equalizer::run");


            for(const Equilibrium::Node *node=cl.standard.head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                eq.display_compact(cl.pad(std::cerr << eq.name, eq) << " : ",C0,TopLevel) << std::endl;
                reac.computeFrom(C0,eq.reac,cl.category);
                prod.computeFrom(C0,eq.prod,cl.category);
                std::cerr << "  |_reac: " << reac << std::endl;
                std::cerr << "  |_prod: " << prod << std::endl;
            }

        }
        
    }

}

