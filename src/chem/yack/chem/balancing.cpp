
#include "yack/chem/balancing.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
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
        xml(out),
        io( new sp_pool() )
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


        static const unsigned is_now_balanced  = 0x00;
        static const unsigned unbalanced_reac  = 0x01;
        static const unsigned unbalanced_prod  = 0x02;
        static const unsigned unbalanced_both  = unbalanced_reac | unbalanced_prod;

#if 0
        static const char * const balanced_msg[] =
        {
            "is now balanced",
            "unbalanced reac",
            "unbalanced prod",
            "unbalanced both"
        };
#endif

        static const char * const balanced_msg[] =
        {
            " [+] ",
            " [<] ",
            " [>] ",
            " [-] "
        };

        
        bool balancing:: balance(writable<double> &C0,
                                 const cluster    &cc)
        {

            YACK_XMLSUB(xml, "balancing:cluster");


            const size_t                     M     = (**this).M ;
            const readable<const criterion> &crit = (**this).crit;

            boundaries boundary_reac(M,io);
            limiting   limiting_reac(io);

            boundaries boundary_prod(M,io);
            limiting   limiting_prod(io);

            sp_repo    vanishing(io);
            
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
            YACK_XMLOG(xml,"-- balancing bounded");
            for(const eq_node *en=cc.bounded->head;en;en=en->next)
            {
                const equilibrium &eq   = **en;
                unsigned           flag = is_now_balanced;

                // interleaved scanning
                fill(boundary_reac,limiting_reac,eq.reac->head,C0,crit);
                fill(boundary_prod,limiting_prod,eq.prod->head,C0,crit);
                if(boundary_reac.size()) flag |= unbalanced_reac;
                if(boundary_prod.size()) flag |= unbalanced_prod;

                if(xml.verbose) eqs.pad(*xml << eq.name,eq) << balanced_msg[flag];


                // check
                switch(flag)
                {
                    case unbalanced_both:
                        if(xml.verbose) std::cerr << std::endl;
                        continue;

                    case unbalanced_reac:
                        assert(limiting_prod.size>0);
                        if(xml.verbose) std::cerr  << boundary_reac << " | limited by: " << limiting_prod << std::endl;
                    {
                        const double xi = boundary_reac.analyze(vanishing,limiting_prod);
                    }
                        break;

                    case unbalanced_prod:
                        assert(limiting_reac.size>0);
                        if(xml.verbose) std::cerr  <<  boundary_prod << " | limited by: " << limiting_reac << std::endl;
                    {
                        const double xi = boundary_prod.analyze(vanishing,limiting_reac);
                    }
                        break;

                    default:
                        assert(0==flag);
                        if(xml.verbose) std::cerr << std::endl;
                        continue;
                }

            }


            return false;
        }

        
    }
}

