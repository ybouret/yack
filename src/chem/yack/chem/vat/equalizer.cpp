#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"

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
        Leqz( speciesFund ),
        Ltmp( speciesFund ),
        reac( speciesFund, cursorsFund   ),
        prod( speciesFund, cursorsFund   ),
        xadd()
        {
        }

        static inline
        void computeLimit(Limit            &lim,
                          const Regulating &regulating,
                          const Equalizing &equalizing)
        {
            lim.initialize();
            std::cerr << " --> equalizing " << equalizing << " with " << regulating << std::endl;
            

        }

        void Equalizer:: run(const xmlog      &xml,
                             writable<double> &C0,
                             const Cluster    &cl)
        {
            YACK_XMLSUB(xml,"Equalizer::run");

            static const unsigned eqz_none = 0x00;
            static const unsigned eqz_reac = 0x01;
            static const unsigned eqz_prod = 0x02;
            static const unsigned eqz_both = eqz_reac | eqz_prod;

            vector<double> Ctmp;
            vector<double> Ceqz;
            Extended::Real Gain;

            for(const Equilibrium::Node *node=cl.standard.head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                reac.computeFrom(C0,eq.reac,cl.category);
                prod.computeFrom(C0,eq.prod,cl.category);

                if(xml.verbose)
                {
                    eq.display_compact(cl.pad( *xml << '<' << eq.name << '>', eq) << " : ",C0,TopLevel) << std::endl;
                    *xml << "  |_reac: " << reac << std::endl;
                    *xml << "  |_prod: " << prod << std::endl;
                }

                unsigned                   eqz_flag  = eqz_none;
                if(reac.equalizing.size>0) eqz_flag |= eqz_reac;
                if(prod.equalizing.size>0) eqz_flag |= eqz_prod;

                switch(eqz_flag)
                {
                    case eqz_none: YACK_XMLOG(xml,"  |_[RUNNING]"); continue;
                    case eqz_both: YACK_XMLOG(xml,"  |_[BLOCKED]"); continue;

                    case eqz_reac: assert(reac.equalizing.size>0); assert(0==prod.equalizing.size); assert(prod.regulating.size>0);
                        YACK_XMLOG(xml,"  |_[(-)REAC]");
                        computeLimit(Leqz,prod.regulating,reac.equalizing);
                        continue;

                    case eqz_prod: assert(prod.equalizing.size>0); assert(0==reac.equalizing.size); assert(reac.regulating.size>0);
                        YACK_XMLOG(xml,"  |_[(-)PROD");
                        continue;

                    default:
                        throw imported::exception("Equalizer","corrupted code");
                }


            }

        }
        
    }

}

