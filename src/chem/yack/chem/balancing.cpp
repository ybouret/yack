
#include "yack/chem/balancing.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/chem/limiting.hpp"
#include "yack/chem/boundary.hpp"
#include <iomanip>


namespace yack
{
    using namespace math;

    namespace chemical
    {

        balancing:: ~balancing() throw()
        {
        }

        balancing:: balancing(const nexus &usr, const xmlog &out) :
        authority<const nexus>(usr),
        xml(out)
        {}

        bool balancing:: operator()(writable<double> &C0)
        {
            bool result = true;
            for(const cluster *cls= (**this).related.head;cls;cls=cls->next)
            {
                if(!balance(C0,*cls)) {
                    result = false;
                }
            }
            return result;
        }






        static inline
        void fill(boundaries                      &neg,
                  limiting                        &pos,
                  const actor                     *a,
                  const readable<double>          &C,
                  const readable<const criterion> &crit)
        {
            neg.destroy();
            pos.destroy();
            for(;a;a=a->next)
            {
                const species &s = **a;
                const size_t   j = *s; if(conserved!=crit[j]) continue;
                const double   c = C[j];
                if(c<0)
                {
                    neg.upgrade( (-c)/a->nu,s );
                }
                else
                {
                    pos.upgrade( c/a->nu,s );
                }
            }
        }
        

        bool balancing:: balance(writable<double> &C0,
                                 const cluster    &cc)
        {

            YACK_XMLSUB(xml, "balancing:cluster");
            static const unsigned balanced         = 0x00;
            static const unsigned unbalanced_reac  = 0x01;
            static const unsigned unbalanced_prod  = 0x02;
            static const unsigned unbalanced_both  = unbalanced_reac | unbalanced_prod;

            const size_t                     M     = (**this).M ;
            const readable<const criterion> &crit = (**this).crit;
            

            
            sp_fund fund = new sp_pool();
            sp_repo zs(fund);
            
            

            boundaries boundary_reac(M,fund);
            limiting   limiting_reac(fund);

            boundaries boundary_prod(M,fund);
            limiting   limiting_prod(fund);

            
            const equilibria &eqs = (**this).lattice;

            YACK_XMLOG(xml,"\\___|cluster| =" << cc.size);
            YACK_XMLOG(xml," \\__|roaming| =" << cc.roaming->size);
            YACK_XMLOG(xml,"  \\_|bounded| =" << cc.bounded->size);

            for(const anode *an = (**this).working.head;an;an=an->next)
            {
                if(C0[***an]<0) goto TRY_BALANCE;
            }
            return true;

        TRY_BALANCE:
            std::cerr << "Try Balancing Bounded" << std::endl;
            for(const eq_node *en=cc.bounded->head;en;en=en->next)
            {
                const equilibrium &eq   = **en;
                unsigned           flag = balanced;
                eqs.pad(std::cerr << "-> " << eq.name,eq) << " : ";

                fill(boundary_reac,limiting_reac,eq.reac->head,C0,crit);
                fill(boundary_prod,limiting_prod,eq.prod->head,C0,crit);
                if(boundary_reac.size()) flag |= unbalanced_reac;
                if(boundary_prod.size()) flag |= unbalanced_prod;
                
                switch(flag)
                {
                    case unbalanced_both:
                        std::cerr << "[unbalanced both]" << std::endl;
                        continue;

                    case unbalanced_reac:
                        std::cerr << "[unbalanced reac] " << boundary_reac << " | limiting: " << limiting_prod;
                        break;

                    case unbalanced_prod:
                        std::cerr << "[unbalanced prod] " << boundary_prod << " | limiting: " << limiting_reac;
                        break;

                    default:
                        assert(0==flag);
                        std::cerr << "[balanced]" << std::endl;
                        continue;
                }



                std::cerr << std::endl;
            }


            return false;
        }

    }
}

