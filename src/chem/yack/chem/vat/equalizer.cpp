#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {


        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const size_t maxClusterSize,
                              const size_t maximumSpecies) :
        speciesFund( new Species::Bank() ),
        cursorsFund( new Cursor::Bank()  ),
        reac( speciesFund, cursorsFund   ),
        prod( speciesFund, cursorsFund   ),
        inUse( maxClusterSize ),
        valid( speciesFund ),
        wrong( speciesFund ),
        nomad( speciesFund ),
        Corg(maximumSpecies,as_capacity),
        Ctmp(maximumSpecies,as_capacity),
        Cend(maximumSpecies,as_capacity),
        Ceqz(maxClusterSize, maxClusterSize ? maximumSpecies : 0),
        xadd()
        {
        }

        void Equalizer:: run(const xmlog      &xml,
                             writable<double> &C0,
                             const Cluster    &cl)
        {
            YACK_XMLSUB(xml,"Equalizer::run");
            runConserved(xml,C0,cl);
        }
        

        void Equalizer:: runConserved(const xmlog      &xml,
                                      writable<double> &C0,
                                      const Cluster    &cluster)
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
                YACK_XMLOG(xml, "[all good]");
                return;
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
                const size_t              ei = eq.indx[SubLevel];

                //--------------------------------------------------------------
                //
                // analyze phase space
                //
                //--------------------------------------------------------------
                reac.computeFrom(Corg,eq.reac,cluster.isRegular,SubLevel);
                prod.computeFrom(Corg,eq.prod,cluster.isRegular,SubLevel);

                if(xml.verbose)
                {
                    eq.display_compact(cluster.pad( *xml << '<' << eq.name << '>', eq) << " : ",Corg,SubLevel) << std::endl;
                    *xml << "  |_reac: " << reac << std::endl;
                    *xml << "  |_prod: " << prod << std::endl;
                }

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
                    case eqz_none: YACK_XMLOG(xml,"  |_[RUNNING]"); continue;
                    case eqz_both: YACK_XMLOG(xml,"  |_[BLOCKED]"); continue;

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
                    YACK_XMLOG(xml, "  |_[STALLED] " << *lim);
                    continue;
                }
                else
                {
                    YACK_XMLOG(xml, "  |_[ USING ] " << *lim);
                }

                //--------------------------------------------------------------
                //
                // compute trial concentrations
                //
                //--------------------------------------------------------------
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
                for(const Species::Node *sn=lim->head;sn;sn=sn->next)
                {
                    Ci[ (***sn).indx[SubLevel] ] = _0;
                }
                if(xml.verbose) eq.display_compact(cluster.pad( *xml << '<' << eq.name << '>', eq) << " : ",Ci,SubLevel) << std::endl;

                //--------------------------------------------------------------
                // register equilibrium
                //--------------------------------------------------------------
                inUse << eq;

            }

            if( inUse.size <= 0)
            {
                throw exception("no equilibria to reduce negativity" );
            }

            YACK_XMLOG(xml, "inUse = " << inUse );
            YACK_XMLOG(xml, " (*) finding best tribe...");
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
            //cluster.for_each_species(std::cerr << "Cend=", "\t[", Cend, "]", SubLevel) << std::endl;;

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

