
#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {

        static inline void computeSide(Equalizing                     &side,
                                       const readable<Extended::Real> &Corg,
                                       const Actors                   &actors)
        {
            assert(actors.size>0);
            side.clear();
            for(const Actor *ac = actors.head; ac; ac=ac->next)
            {
                const Extended::Real c = Corg[ (**ac).indx[SubLevel] ];
                if(c.m<0)
                    side.tryInsertCursor(-c, *ac);
            }
        }

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

            cluster.for_each_species(std::cerr << "Corg=", "\t[", Corg, "]", SubLevel) << std::endl;;

            for(const Equilibrium::Node *en=cluster.reacOnly.head;en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                computeSide(side,Corg,eq.reac);
                if(side.size)
                {
                    Cursor &cursor = **side.tail;
                    cursor.neg();
                    eq.move(Corg,SubLevel,cursor);
                    cursor.nullify(Corg,SubLevel);
                    cluster.for_each_species(std::cerr << eq << " -> ", "\t[", Corg, "]", SubLevel) << std::endl;;
                }
            }


            for(const Equilibrium::Node *en=cluster.prodOnly.head;en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                computeSide(side,Corg,eq.prod);
                if(side.size)
                {
                    Cursor &cursor = **side.tail;
                    eq.move(Corg,SubLevel,cursor);
                    cursor.nullify(Corg,SubLevel);
                    cluster.for_each_species(std::cerr << eq << " -> ", "\t[", Corg, "]", SubLevel) << std::endl;;
                }
            }





        }

    }

}


