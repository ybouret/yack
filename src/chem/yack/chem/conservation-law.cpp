
#include "yack/chem/conservation-law.hpp"

namespace yack
{
    namespace chemical
    {

        conservation_law:: ~conservation_law() throw() {}
        conservation_law::  conservation_law() throw() : object(), actors(), next(0), prev(0) {}


        std::ostream & operator<<(std::ostream &os, const conservation_law &self)
        {
            const actors &me = self;
            os << "0=d(" << me << ")";
            return os;
        }

    }

}

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


namespace yack
{
    namespace chemical
    {

        claw_teams :: claw_teams() throw() : claw_teams_()
        {

        }

        claw_teams:: ~claw_teams() throw()
        {

        }

        void claw_teams:: recruit(const conservation_law &law)
        {
            if(merged(law))
            {
                reduce();
            }
        }


        bool claw_teams:: merged(const conservation_law &law)
        {
            for(claw_team *team=head;team;team=team->next)
            {
                if(team->attached_to(law))
                {
                    // found shared species: append law to same team
                    (*team) << &law;
                    return true;
                }
            }

            // create a new team
            (*push_back(new claw_team())) << &law;
            return false;
        }

        void claw_teams:: reduce() throw()
        {
            claw_teams_ interim;
            while(size)
            {
                // take first node, declared as not stolen
                claw_team *curr   = pop_front(); assert(curr->size>0);
                bool       stolen = false;

                for(claw_team *team=interim.head;team;team=team->next)
                {
                    if(team->attached_to(*curr))
                    {
                        // this team steals current node
                        team->merge_back(*curr);
                        stolen = true;
                        break;
                    }
                }

                if(stolen)
                {
                    // current team is empty
                    assert(0==curr->size);
                    delete curr;
                }
                else
                {
                    // untouched team
                    interim.push_back(curr);
                }
            }
            swap_with(interim);
        }

    }
}
