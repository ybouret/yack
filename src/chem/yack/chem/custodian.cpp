
#include "yack/chem/custodian.hpp"
#include <iomanip>


namespace yack
{
    
    namespace chemical
    {
        
        custodian:: ~custodian() throw()
        {
        }
        

        custodian:: custodian(const nexus &usr) :
        heart(usr),
        count(heart.Nq),
        score(count),
        state(count,count>0?heart.M:0),
        alive(count)
        {
        }

        void custodian:: abide(writable<double> &C0,
                               const claw_team  &claws)
        {
            assert(C0.size()>=heart.M);
            score.ld(0);
            alive.free();

            // initialize
            for(const claw_node *node=claws.head;node;node=node->next)
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

            while(alive->size)
            {
                std::cerr << "processing " << *alive << std::endl;
                break;
            }


        }


        void custodian:: abide(writable<double>  &C0,
                               const cluster     &cc)
        {
            for(const claw_team *team=cc.cells.head;team;team=team->next)
            {
                abide(C0,*team);
            }
        }

        void custodian::  operator()(writable<double> &C0)
        {
            for(const cluster *cls=heart.related.head;cls;cls=cls->next)
            {
                abide(C0,*cls);
            }
        }



    }
    
}
