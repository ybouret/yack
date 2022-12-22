
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

        


    }

}


