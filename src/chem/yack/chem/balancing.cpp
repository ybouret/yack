
#include "yack/chem/balancing.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/type/utils.hpp"
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
        spIO( new sp_pool() ),
        reac(usr.M,spIO),
        prod(usr.M,spIO),
        Cbalanced(usr.L,usr.L>0?usr.M:0),
        vanishing(spIO),
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


        static const unsigned is_now_balanced  = 0x00;
        static const unsigned unbalanced_reac  = 0x01;
        static const unsigned unbalanced_prod  = 0x02;
        static const unsigned unbalanced_both  = unbalanced_reac | unbalanced_prod;
        
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

            boundaries boundary_reac(M,spIO);
            limiting   limiting_reac(spIO);

            boundaries boundary_prod(M,spIO);
            limiting   limiting_prod(spIO);
            
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



                // check
                switch(flag)
                {
                    case unbalanced_both:
                        continue;

                    case unbalanced_reac:
                        assert(limiting_prod.size>0);
                        boundary_reac.analyze(vanishing,limiting_prod);
                        if(xml.verbose) eqs.pad(*xml << eq.name,eq) << balanced_msg[flag]  << boundary_reac << " | limited by prod: " << limiting_prod << " => " << vanishing << std::endl;
                        coerce(vanishing.xi) = -vanishing.xi;
                        std::cerr << "score= " << score(C0,eq) << std::endl;
                        break;

                    case unbalanced_prod:
                        assert(limiting_reac.size>0);
                        boundary_prod.analyze(vanishing,limiting_reac);
                        if(xml.verbose) eqs.pad(*xml << eq.name,eq) << balanced_msg[flag] <<  boundary_prod << " | limited by reac: " << limiting_reac << " => " << vanishing << std::endl;
                        std::cerr << "score= " << score(C0,eq) << std::endl;
                        break;

                    default:
                        assert(0==flag);
                        continue;
                }


            }


            return false;
        }


        double balancing:: score(const readable<double> &C0, const equilibrium &eq)
        {
            writable<double> &C = Cbalanced[*eq];

            // first pass, evaluate best concentration
            iota::load(C,C0);

            const double xi = vanishing.xi;
            for(const cnode *cn=eq.head();cn;cn=cn->next)
            {
                const component &a = ***cn;
                const species   &s = *a;
                const size_t     j = *s;
                const double     c = C0[j];
                if(c<0)
                {
                    C[j] = C0[j] + a.nu * xi;
                }
                else
                {
                    C[j] = max_of(C0[j]+a.nu*xi,0.0);
                }
            }

            for(const sp_knot *sn=vanishing.head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                const size_t   j  = *sp;
                C[j] = 0;
            }

            (**this).corelib(std::cerr,"",C);


            const readable<const criterion> &crit = (**this).crit;
            xadd.ldz();

            // second pass, get improvement
            for(const cnode *cn=eq.head();cn;cn=cn->next)
            {
                const size_t j = *****cn;
                if(crit[j]!=conserved) continue;
                const double c0 = C0[j];
                if(c0<0)
                {
                    xadd.push(-c0);
                    const double c = C[j];
                    if(c<0) xadd.push(c);
                }
            }



            return xadd.get();
        }
        
    }
}

