
#include "yack/chem/realm.hpp"

namespace yack
{
    namespace chemical
    {
        realm:: ~realm() noexcept
        {
        }

        realm:: realm(const library          &lib_,
                      const equilibria       &eqs_,
                      const readable<double> &eks) :
        domains(),
        xmlog("[realm]",std::cerr,species::verbose),
        lib(lib_),
        eqs(eqs_),
        reg(lib->size,true),
        grp(lib->size,0),
        Nu(eqs->size,eqs->size>0?lib->size:0),
        NuT(Nu.cols,Nu.rows)
        {
            const xmlog &xml = *this;
            //------------------------------------------------------------------
            //
            // make domains of independents equilibria
            //
            //------------------------------------------------------------------
            build_domains(xml);
            for(const enode *en=eqs->head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = eq.indx[top_level];
                eq.fill(top_level,coerce(Nu[ei]));
            }
            coerce(NuT).assign(Nu,transposed);

            //------------------------------------------------------------------
            //
            // create partitions
            //
            //------------------------------------------------------------------
            int    iviz = 0;
            size_t igrp = 0;
            for(domain *dom=head;dom;dom=dom->next)
            {
                dom->create(xml,eqs,eks);
                coerce(dom->iviz) = iviz++;
                for(const conservation *cv=dom->laws.head;cv;cv=cv->next)
                {
                    ++igrp;
                    for(const actor *a=cv->head;a;a=a->next)
                    {
                        coerce(grp[(**a).indx[top_level]]) = igrp;
                    }
                }
            }

            lib(std::cerr << "reg=",reg) << std::endl;

        }

        size_t realm:: largest_domain_size() const noexcept
        {
            size_t res = 0;
            for(const domain *dom=head;dom;dom=dom->next)
            {
                res = max_of(res,dom->size);
            }
            return res;
        }


        void   realm:: prepareK(sequence<double> &K) const
        {
            K.make(eqs->size,0);
        }


        void realm:: computeK(writable<double> &K, const double t)
        {
            assert(K.size()>=eqs->size);

            for( enode *en=eqs->head;en;en=en->next)
            {
                equilibrium &eq = ***en;
                K[ eq.indx[top_level] ] = eq.K(t);
            }
        }
        
    }
    
}

#include "yack/chem/realm/claw/custodian.hpp"

namespace yack
{
    namespace chemical
    {

        void custodian:: process(const xmlog &xml,
                                 writable<double>  &C,
                                 const domain      &dom)
        {
            for(const conserved *cns=dom.pack.head;cns;cns=cns->next)
            {
                process(xml,C,*cns);
            }
        }

        void custodian:: process(const realm      &chem,
                                 writable<double> &conc)
        {
            const xmlog &xml = chem;
            YACK_XMLSUB(xml,"custodian::process");
            prepare();
            for(const domain *dom = chem.head;dom;dom=dom->next)
                process(xml,conc,*dom);
            
        }
    }
}

#include "yack/chem/realm/collector.hpp"

namespace yack
{
    namespace chemical
    {

        void collector::  balance(const realm      &chem,
                                  writable<double> &conc)
        {
            ld(0);
            for(const domain *dom=chem.head;dom;dom=dom->next)
            {
                balance(chem,*dom,conc);
            }
        }
    }
}


