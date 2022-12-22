
#include "yack/chem/claw/teams.hpp"
#include <iomanip>

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
