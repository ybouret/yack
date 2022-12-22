
#include "yack/chem/custodian.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include <iomanip>


namespace yack
{
    using namespace math;

    namespace chemical
    {
        
        custodian:: ~custodian() throw()
        {
        }
        

        custodian:: custodian(const nexus &usr, const xmlog &out) :
        authority<const nexus>(usr),
        rules( (**this).Nq ),
        items( (**this).M  ),
        score(rules),
        inset(items),
        state(rules,rules>0? items:0 ),
        alive(rules),
        xml(out)
        {
        }


        bool custodian::  initialize_with(const readable<double> &C0,
                                          const claw_team        &ct)
        {
            assert(C0.size()>= (**this).M);
            score.ld(0);
            alive.free();
            for(const claw_node *node=ct.head;node;node=node->next)
            {
                const conservation_law &claw = **node;
                const size_t            i    = *claw;
                if( claw.regulate(state[i],C0,xadd) )
                {
                    score[i] = xadd.get();
                    alive.push_back(claw);
                    std::cerr << std::setw(15) << score[i] << " @" << claw << std::endl;
                }
                else
                {
                    std::cerr << std::setw(15) << "ok" << " @" << claw << std::endl;
                }
            }
            return (alive->size > 0);
        }


        claw_node * custodian:: select_next_law() throw()
        {
            assert(alive->size>0);
            claw_node *low_node = alive->head;
            double     low_corr = score[***low_node];
            for(claw_node *node=low_node->next;node;node=node->next)
            {
                const double corr = score[***node];
                if(corr<low_corr) {
                    low_corr = corr;
                    low_node = node;
                }
            }
            return low_node;
        }


        bool custodian:: need_to_process(const readable<double> &C0)
        {

            {
                claw_repo::list_type still_alive;
                while(alive->size)
                {
                    auto_ptr<claw_node>     node = alive->pop_front();
                    const conservation_law &claw = **node;
                    const size_t            indx = *claw;
                    if( claw.regulate(state[indx],C0,xadd) )
                    {
                        score[indx] = xadd.get();
                        still_alive.push_back(node.yield());
                        std::cerr << std::setw(15) << score[indx] << " @" << claw << std::endl;
                    }
                    else
                    {
                        alive.zstore(node.yield());
                        std::cerr << std::setw(15) << "ok" << " @" << claw << std::endl;
                    }
                }
                alive.list.swap_with(still_alive);
            }
            return alive->size>0;
        }


        void custodian:: abide(writable<double> &C0,
                               const claw_team  &claws)
        {
            YACK_XMLSUB(xml, "custodian:cluster:team");
            if( initialize_with(C0,claws) )
            {
            PROCESSING:
                // find minimal displacement
                std::cerr << "processing " << *alive << std::endl;
                claw_node              *node = select_next_law();
                const conservation_law &claw = **node;
                const size_t            indx = *claw;
                std::cerr << "low: " << claw << " with " << score[indx] << std::endl;
                std::cerr << "from " << C0 << " to " << state[indx] << std::endl;

                // remove satisfied rule
                iota::save(C0,state[indx]);
                score[indx] = 0;
                alive.pop(node);

                if( need_to_process(C0) )
                    goto PROCESSING;

            }


        }


        void custodian:: abide(writable<double>  &C0,
                               const cluster     &cc)
        {
            YACK_XMLSUB(xml, "custodian:cluster");
            for(const claw_team *team=cc.cells.head;team;team=team->next)
            {
                abide(C0,*team);
            }
        }

        void custodian::  operator()(writable<double> &C0)
        {
            YACK_XMLSUB(xml, "custodian");
            inset.ld(0);
            for(const cluster *cls=(**this).related.head;cls;cls=cls->next)
            {
                abide(C0,*cls);
            }
        }



    }
    
}
