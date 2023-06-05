#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
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
        Corg(maximumSpecies,as_capacity),
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
                                      const Cluster    &cl)
        {
            YACK_XMLSUB(xml,"Equalizer::runConserved");

            static const unsigned eqz_none = 0x00;
            static const unsigned eqz_reac = 0x01;
            static const unsigned eqz_prod = 0x02;
            static const unsigned eqz_both = eqz_reac | eqz_prod;

            const size_t m    = C0.size();
            Corg.adjust(m,_0);

            //------------------------------------------------------------------
            //
            // computing cbad
            //
            //------------------------------------------------------------------
            double       cbad = 0;
            for(const Species::Node *node=cl.lib.head;node;node=node->next)
            {
                const Species       &sp  = ***node;
                const double         cj  = C0[sp.indx[TopLevel]];
                Corg[sp.indx[SubLevel]]  = Extended::Send(cj);
                if(cl.isUnbounded(sp)) continue;
                if(cj<0)
                {
                    cbad -= cj;
                }
            }

            if(cbad<=0)
            {
                YACK_XMLOG(xml, "[all good]");
                return;
            }

            //------------------------------------------------------------------
            //
            // computing optimal extents for each eq
            //
            //------------------------------------------------------------------
            const Equilibrium *best = NULL;
            Extended::Real     gain = _0;

            for(const Equilibrium::Node *node=cl.standard.head;node;node=node->next)
            {
                const Equilibrium        &eq = ***node;
                const size_t              ei = eq.indx[SubLevel];
                writable<Extended::Real> &Ci = Ceqz[ei];

                //--------------------------------------------------------------
                // analyze phase space
                //--------------------------------------------------------------
                reac.computeFrom(Corg,eq.reac,cl.category,SubLevel);
                prod.computeFrom(Corg,eq.prod,cl.category,SubLevel);

                if(xml.verbose)
                {
                    eq.display_compact(cl.pad( *xml << '<' << eq.name << '>', eq) << " : ",Corg,SubLevel) << std::endl;
                    *xml << "  |_reac: " << reac << std::endl;
                    *xml << "  |_prod: " << prod << std::endl;
                }

                unsigned                   eqz_flag  = eqz_none;
                if(reac.equalizing.size>0) eqz_flag |= eqz_reac;
                if(prod.equalizing.size>0) eqz_flag |= eqz_prod;

                //--------------------------------------------------------------
                // decide what to do according to analysis
                //--------------------------------------------------------------
                Limit *lim = NULL;
                switch(eqz_flag)
                {
                    case eqz_none: YACK_XMLOG(xml,"  |_[RUNNING]"); continue;
                    case eqz_both: YACK_XMLOG(xml,"  |_[BLOCKED]"); continue;

                    case eqz_reac: assert(reac.equalizing.size>0); assert(0==prod.equalizing.size); assert(prod.regulating.size>0);
                        //YACK_XMLOG(xml,"  |_[(-)REAC]");
                        reac.equalizing.findBestEffort(prod.regulating);
                        prod.regulating.neg();
                        lim = & prod.regulating;
                        break;

                    case eqz_prod: assert(prod.equalizing.size>0); assert(0==reac.equalizing.size); assert(reac.regulating.size>0);
                        //YACK_XMLOG(xml,"  |_[(-)PROD]");
                        prod.equalizing.findBestEffort(reac.regulating);
                        lim = & reac.regulating;
                        break;

                    default:
                        throw imported::exception("Equalizer","corrupted code");
                }
                assert(lim);
                YACK_XMLOG(xml, "  |_[ USING ] " << *lim);

                //--------------------------------------------------------------
                // compute trial concentration 
                //--------------------------------------------------------------
                const Extended::Real      xi   = **lim;
                for(const cNode *cn=eq->head;cn;cn=cn->next)
                {
                    const Component     &cc   = ***cn;
                    const Species       &sp   = *cc;
                    const size_t         sj   = sp.indx[SubLevel];
                    const Extended::Real oldC = Corg[sj];
                    const Extended::Real dC   = cc.xn * xi;
                    const Extended::Real newC = oldC+dC;
                    if(cl.isUnbounded(sp))
                    {
                        Ci[sj] = newC;
                    }
                    else
                    {
                        assert(cl.isConserved(sp));
                        if(oldC>=0)
                        {
                            Ci[sj] = max_of(newC,_0);
                        }
                        else
                        {
                            Ci[sj] = newC;
                        }
                    }
                }

                //--------------------------------------------------------------
                // make vanishing species
                //--------------------------------------------------------------
                for(const Species::Node *sn=lim->head;sn;sn=sn->next)
                {
                    Ci[ (***sn).indx[SubLevel] ] = _0;
                }
                eq.display_compact(cl.pad( *xml << '<' << eq.name << '>', eq) << " : ",Ci,SubLevel) << std::endl;

                //--------------------------------------------------------------
                // compute gain
                //--------------------------------------------------------------
                xadd.resume(m);
                for(const Species::Node *sn=cl.conserved.head;sn;sn=sn->next)
                {
                    const size_t         j = (***sn).indx[SubLevel];
                    const Extended::Real oldV = -min_of(Corg[j], _0);
                    const Extended::Real newV =  min_of(Ci[j],   _0);
                    xadd.append(oldV);
                    xadd.append(newV);
                }
                const Extended::Real g = xadd.reduce().abs();
                std::cerr << " --> gain=" << g << std::endl;
                if(!best || g<gain)
                {
                    best = &eq;
                    gain =  g;
                }
            }

            if(best)
            {
                std::cerr << "best=" << best->name << std::endl;
            }
            else
            {
                std::cerr << "no possible..." << std::endl;
            }


        }
        
    }

}

