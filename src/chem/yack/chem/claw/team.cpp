
#include "yack/chem/claw/team.hpp"
#include <iomanip>

namespace yack
{
    
    namespace chemical
    {
        claw_team :: claw_team() throw() : next(0), prev(0)
        {

        }

        claw_team:: ~claw_team() throw()
        {

        }

        bool claw_team:: attached_to(const conservation_law &lhs) const throw()
        {
            for(const claw_node *node=head;node;node=node->next)
            {
                const conservation_law &rhs = **node;
                if(rhs.attached_to(lhs)) return true;
            }
            return false;
        }

        bool claw_team::  attached_to(const claw_team &team) const throw()
        {
            for(const claw_node *node=head;node;node=node->next)
            {
                if(team.attached_to(**node)) return true;
            }
            return false;
        }


        void claw_team :: update(writable<double> &C0,
                                 matrix<double>   &Qc,
                                 writable<double> &Qs,
                                 claw_repo        &cr,
                                 raddops          &xadd) const
        {
            std::cerr << "updating #" << size << " team(s)" << std::endl;

            // initialize
            cr.free();
            for(const claw_node *node=head;node;node=node->next)
            {
                const conservation_law &claw = **node;
                const size_t            indx = *claw;
                if( claw.regulate(Qc[indx],C0,xadd) )
                {
                    Qs[indx] = xadd.get();
                    cr.push_back(claw);
                    std::cerr << std::setw(15) <<  Qs[indx] << "@ " << claw << std::endl;
                }
            }
            

        }


    }

}


