
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


        bool balancing:: balance(writable<double> &C0,
                                 const cluster    &cc)
        {

            static const unsigned balanced         = 0x00;
            static const unsigned unbalanced_reac  = 0x01;
            static const unsigned unbalanced_prod  = 0x02;
            static const unsigned unbalanced_both  = unbalanced_reac | unbalanced_prod;


            const equilibria &eqs = (**this).lattice;

            YACK_XMLOG(xml,"balancing |cluster| =" << cc.size);
            YACK_XMLOG(xml,"          |roaming| =" << cc.roaming->size);
            YACK_XMLOG(xml,"          |bounded| =" << cc.bounded->size);

            for(const anode *an = (**this).working.head;an;an=an->next)
            {
                if(C0[***an]<0) goto TRY_BALANCE;
            }
            return true;

        TRY_BALANCE:
            // try bounded eqs of this cluster
            std::cerr << "Try Balancing Bounded" << std::endl;
            for(const eq_node *en=cc.bounded->head;en;en=en->next)
            {
                const equilibrium &eq   = **en;
                unsigned           flag = balanced;
                eqs.pad(std::cerr << "-> " << eq.name,eq) << " : ";

                for(const actor *a=eq.reac->head;a;a=a->next)
                {
                    const species &s = **a;
                    const size_t   j = *s;
                    const double   c = C0[j];
                    if(c<0)
                    {
                        flag |= unbalanced_reac;
                    }
                    else
                    {

                    }
                }

                for(const actor *a=eq.prod->head;a;a=a->next)
                {
                    const species &s = **a;
                    const size_t   j = *s;
                    const double   c = C0[j];
                    if(c<0)
                    {
                        flag |= unbalanced_prod;
                    }
                    else
                    {

                    }
                }

                switch(flag)
                {
                    case unbalanced_both:
                        std::cerr << "[unbalanced both]";
                        break;

                    case unbalanced_reac:
                        std::cerr << "[unbalanced reac]";
                        break;

                    case unbalanced_prod:
                        std::cerr << "[unbalanced prod]";
                        break;

                    default:
                        assert(0==flag);
                        std::cerr << "[balanced]";


                }




                eq.display_compact(std::cerr,C0);
                std::cerr << std::endl;
            }


            return false;
        }

    }
}

