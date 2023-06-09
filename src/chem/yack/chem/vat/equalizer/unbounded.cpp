
#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {

        void Equalizer:: runUnbounded(const xmlog               &xml,
                                      writable<Extended::Real> &C0,
                                      const Cluster            &cluster)
        {
            YACK_XMLSUB(xml,"Equalizer::runUbounded");
            assert(cluster.lib.size == (***cluster.lib.tail).indx[SubLevel]);

            //------------------------------------------------------------------
            //
            // adjusting workspace
            //
            //------------------------------------------------------------------
            const size_t m = cluster.lib.size;
            Corg.adjust(m,_0);
            Ctmp.adjust(m,_0);
            Cend.adjust(m,_0);
            cluster.load(Corg,C0);

            valid.clear();
            nomad.clear();


            wrong.clear();
            for(const Species::Node *sn=cluster.unbounded.head;sn;sn=sn->next)
            {
                const Species &sp = ***sn;
                if(Corg[ sp.indx[SubLevel]] < _0) wrong << sp;
            }

            if(wrong.size<=0)
            {
                YACK_XMLOG(xml, "[all good unbounded]");
                return;
            }



            for(const Equilibrium::Node *en=cluster.reacOnly.head;en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                //--------------------------------------------------------------
                //
                // analyze phase space
                //
                //--------------------------------------------------------------
                reac.computeFrom(Corg,eq.reac,SubLevel);
                //prod.computeFrom(Corg,eq.prod,SubLevel);
                std::cerr << eq << std::endl;
                std::cerr << "reac: " << reac << std::endl;
                //std::cerr << "prod: " << prod << std::endl;
            }

            for(const Equilibrium::Node *en=cluster.prodOnly.head;en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                //--------------------------------------------------------------
                //
                // analyze phase space
                //
                //--------------------------------------------------------------
                //reac.computeFrom(Corg,eq.reac,SubLevel);
                prod.computeFrom(Corg,eq.prod,SubLevel);
                std::cerr << eq << std::endl;
                //std::cerr << "reac: " << reac << std::endl;
                std::cerr << "prod: " << prod << std::endl;
            }


        }

    }

}


