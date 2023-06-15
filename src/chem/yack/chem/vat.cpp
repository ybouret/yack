#include "yack/chem/vat.hpp"

namespace yack
{
    namespace Chemical
    {
        Vat:: ~Vat() noexcept
        {
        }

        Vat:: Vat(const xmlog      &xml,
                  Equilibria       &eqs) :
        Clusters(),
        maxClusterSize(0),
        maximumSpecies(0)
        {
            makeClusters(xml,eqs,coerce(K));
            for(const Cluster *cl=head;cl;cl=cl->next)
            {
                assert(cl->size);
                assert(cl->lib.size);
                coerce(maxClusterSize) = max_of(maxClusterSize,cl->size);
                coerce(maximumSpecies) = max_of(maximumSpecies,cl->lib.size);
            }
        }


        void Vat:: displayK(std::ostream &os) const
        {
            for(const Cluster *cl=head;cl;cl=cl->next)
                cl->displayK(os,K);
        }

        void Vat:: displayS(std::ostream &os) const
        {
            for(const Cluster *cl=head;cl;cl=cl->next)
                cl->displayS(os,S);
        }



    }

}

