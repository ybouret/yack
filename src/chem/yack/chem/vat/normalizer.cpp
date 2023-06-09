
#include "yack/chem/vat/normalizer.hpp"

namespace yack
{
    namespace Chemical
    {
        Normalizer:: ~Normalizer() noexcept
        {
        }

        
        Normalizer:: Normalizer(const size_t maxClusterSize,
                                const size_t maximumSpecies) :
        custodian(maximumSpecies),
        equalizer(maxClusterSize,maximumSpecies)
        {

        }

        void Normalizer:: process(const xmlog              &xml,
                                  writable<Extended::Real> &C0,
                                  const Cluster            &cluster)
        {
            
            while(!equalizer.runConserved(xml,C0,cluster))
                custodian.enforce(xml,C0,cluster);

            equalizer.runUnbounded(xml,C0,cluster);
        }

        void Normalizer:: operator()(const xmlog              &xml,
                                     writable<Extended::Real> &C0,
                                     const list_of<Cluster>   &clusters)
        {
            custodian.restart(C0.size());
            for(const Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
                process(xml,C0,*cluster);
            if(xml.verbose)
            {
                for(const Cluster *cluster=clusters.head;cluster;cluster=cluster->next)
                {
                    cluster->for_each_species(*xml << "injected = ", "\td[", custodian,"]", TopLevel) << std::endl;
                }
            }
        }


    }

}
