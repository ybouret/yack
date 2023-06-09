#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;
    
    namespace Chemical
    {

        static inline
        void displayStatus(const xmlog                    &xml,
                           const Equilibrium              &E,
                           const readable<Extended::Real> &C,
                           const Cluster                  &cluster,
                           const char                     *status,
                           const bool                      newline=true)
        {
            if(xml.verbose)
            {
                E.display_compact(cluster.pad( *xml << "  |_[" <<status << "] " << '<' << E.name << '>', E) << " : ",C,SubLevel);
                if(newline) xml() << std::endl;
            }
        }


        bool Equalizer:: runConserved(const xmlog              &xml,
                                      writable<Extended::Real> &C0,
                                      const Cluster            &cluster)
        {
            YACK_XMLSUB(xml,"Equalizer::runConserved");
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

            //------------------------------------------------------------------
            //
            // categorizing species
            //
            //------------------------------------------------------------------
            unsigned cycle=0;
        CYCLE:
            ++cycle;
            std::cerr << "---------- cycle " << cycle << " ----------" << std::endl;
            cluster.for_each_species(std::cerr << "Corg=", "\t[", Corg, "]", SubLevel) << std::endl;;

            valid.clear();
            wrong.clear();
            nomad.clear();
            inUse.clear();
            for(const Species::Node *node=cluster.lib.head;node;node=node->next)
            {
                const Species       &sp = ***node;
                const Extended::Real cj = Corg[sp.indx[SubLevel]];
                if(cluster.isConserved(sp))
                {
                    if(cj<_0)
                    {
                        wrong << sp;
                    }
                    else
                    {
                        valid << sp; //!< will stay valid
                    }
                }
                else
                {
                    nomad << sp;
                }

            }

            if(wrong.size<=0)
            {
                YACK_XMLOG(xml, "[all good conserved]");
                if(cycle>1)
                    cluster.save(C0,Corg);
                return true;
            }

            YACK_XMLOG(xml," (*) wrong = " << wrong);
            YACK_XMLOG(xml," (*) valid = " << valid);
            YACK_XMLOG(xml," (*) nomad = " << nomad);

            //------------------------------------------------------------------
            //
            // detect equilibria that can reduce negativity
            //
            //------------------------------------------------------------------
            static const unsigned eqz_none = 0x00;
            static const unsigned eqz_reac = 0x01;
            static const unsigned eqz_prod = 0x02;
            static const unsigned eqz_both = eqz_reac | eqz_prod;


            for(const Equilibrium::Node *node=cluster.standard.head;node;node=node->next)
            {
                const Equilibrium        &eq = ***node;

                //--------------------------------------------------------------
                //
                // analyze phase space
                //
                //--------------------------------------------------------------
                reac.computeFrom(Corg,eq.reac,cluster.isRegular,SubLevel);
                prod.computeFrom(Corg,eq.prod,cluster.isRegular,SubLevel);


                unsigned                   eqz_flag  = eqz_none;
                if(reac.equalizing.size>0) eqz_flag |= eqz_reac;
                if(prod.equalizing.size>0) eqz_flag |= eqz_prod;

                //--------------------------------------------------------------
                //
                // decide what to do according to analysis
                //
                //--------------------------------------------------------------
                Limit *lim = NULL;
                switch(eqz_flag)
                {
                    case eqz_none: displayStatus(xml, eq, Corg, cluster, "RUNNING" ); continue;
                    case eqz_both: displayStatus(xml, eq, Corg, cluster, "BLOCKED" ); continue;

                    case eqz_reac: assert(reac.equalizing.size>0); assert(0==prod.equalizing.size); assert(prod.regulating.size>0);
                        reac.equalizing.findBestEffort(prod.regulating);
                        prod.regulating.neg();
                        lim = & prod.regulating;
                        break;

                    case eqz_prod: assert(prod.equalizing.size>0); assert(0==reac.equalizing.size); assert(reac.regulating.size>0);
                        prod.equalizing.findBestEffort(reac.regulating);
                        lim = & reac.regulating;
                        break;

                    default:
                        throw imported::exception("Equalizer","corrupted code");
                }
                assert(lim);
                const Extended::Real xi = **lim;
                if( xi.abs().m <= 0 )
                {
                    displayStatus(xml, eq, Corg, cluster, "STALLED" );
                    continue;
                }
#if 0
                else
                {
                    displayStatus(xml, eq, Corg, cluster, " USING " );
                    YACK_XMLOG(xml, "   |_[ " << *lim << " ]" );
                }
#endif

                //--------------------------------------------------------------
                //
                // compute trial concentrations
                //
                //--------------------------------------------------------------
                const size_t              ei = eq.indx[SubLevel];
                writable<Extended::Real> &Ci = Ceqz[ei];
                keto::load(Ci,Corg);
                eq.move(Ci,SubLevel,xi);

                //--------------------------------------------------------------
                // take care of valid species
                //--------------------------------------------------------------
                for(const Species::Node *sn=valid.head;sn;sn=sn->next)
                {
                    const size_t j = (***sn).indx[SubLevel];
                    Ci[j] = max_of(Ci[j],_0);
                }

                //--------------------------------------------------------------
                // make vanishing species
                //--------------------------------------------------------------
                lim->nullify(Ci,SubLevel);
                displayStatus(xml, eq, Ci, cluster, " USING ", false);
                if(xml.verbose) xml() << " <-- " << *lim << std::endl;

                //--------------------------------------------------------------
                // register equilibrium
                //--------------------------------------------------------------
                inUse << eq;

            }

            if( inUse.size <= 0)
            {
                YACK_XMLOG(xml, "[still negative: " << wrong << "]");
                if(cycle>1)
                    cluster.save(C0,Corg);
                return false;
            }

            YACK_XMLOG(xml, " (*) => " << inUse );
            YACK_XMLOG(xml, " (*) => finding best tribe:");
            //------------------------------------------------------------------
            //
            // find out the best gain
            //
            //------------------------------------------------------------------
            {
                //--------------------------------------------------------------
                // initialize to no gain @Corg
                //--------------------------------------------------------------
                Extended::Real gain = -1;
                keto::load(Cend,Corg);

                //--------------------------------------------------------------
                // try each compatible tribe
                //--------------------------------------------------------------
                for(const Tribe *tribe=cluster.equalizing.head;tribe;tribe=tribe->next)
                {
                    if(tribe->is_subset_of(inUse))
                    {
                        const Extended::Real g   = gainOf(*tribe,cluster);
                        if(g>gain)
                        {
                            gain = g;
                            keto::load(Cend,Ctmp);
                            YACK_XMLOG(xml, " (+) " <<  std::setw(15) << *g  << " " << *tribe << " @" << g);
                        }
                    }
                }

                assert(gain>=0);
            }

            //------------------------------------------------------------------
            //
            // update Corg
            //
            //------------------------------------------------------------------
            keto::load(Corg,Cend);
            goto CYCLE;
        }


        Extended::Real Equalizer:: gainOf(const Tribe   &tribe,
                                          const Cluster &cluster)
        {
            assert(Corg.size()==Cend.size());
            assert(Corg.size()==Ctmp.size());
            assert(cluster.lib.size == Corg.size());


            //------------------------------------------------------------------
            //
            // initialize Ctmp
            //
            //------------------------------------------------------------------
            keto::load(Ctmp,Corg);

            //------------------------------------------------------------------
            //
            // compute Cend for each species in tribe
            // using precomputed concentrations to deduce
            // sum of extents
            //
            //------------------------------------------------------------------
            for(const Species::Node *sn=tribe.lib.head;sn;sn=sn->next)
            {
                const Species        &sp = ***sn;
                const size_t          j  = sp.indx[SubLevel];
                const Extended::Real  c0 = Corg[j];
                const Extended::Real  m0 = -c0;
                xadd.free();
                xadd.push(c0);
                for(const Equilibrium::Node *en=tribe.head;en;en=en->next)
                {
                    const Equilibrium &eq = ***en;
                    const size_t       i  = eq.indx[SubLevel];
                    xadd.push(m0);
                    xadd.push(Ceqz[i][j]);
                }
                Ctmp[j] = xadd.reduce();
            }

            // regularizing valid species
            for(const Species::Node *sn=valid.head;sn;sn=sn->next)
            {
                const Species  &sp = ***sn;
                const size_t    j  = sp.indx[SubLevel];
                Extended::Real &c  = Ctmp[j];
                if(c<=_0) c=_0;
            }



            //------------------------------------------------------------------
            //
            // compute gain
            //
            //------------------------------------------------------------------
            xadd.free();
            for(const Species::Node *sn=wrong.head;sn;sn=sn->next)
            {
                const Species        &sp = ***sn;
                const size_t         j    = sp.indx[SubLevel];
                const Extended::Real oldV =  min_of(Corg[j], _0);
                const Extended::Real newV =  min_of(Ctmp[j], _0);
                assert(newV>=oldV);
                const Extended::Real gain = (oldV-newV).abs();
                xadd.append(gain);
            }


            return xadd.reduce();
        }

        
    }
    
}
