
#include "yack/chem/conservation-law.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        conservation_law:: ~conservation_law() throw() {}
        conservation_law::  conservation_law() throw() :
        object(),
        actors(),
        next(0),
        prev(0),
        nrm2(0),
        indx(0)
        {}


        std::ostream & operator<<(std::ostream &os, const conservation_law &self)
        {
            const actors &me = self;
            os << "0=d(" << me << ")";
            return os;
        }

        void   conservation_law:: finalize(const size_t i) throw()
        {
            coerce(nrm2) = 0;
            coerce(indx) = i; assert(indx>0);
            for(const actor *a=crew.head;a;a=a->next)
            {
                coerce(nrm2) += squared(a->nu);
            }

        }

        double conservation_law:: evaluate(const readable<double> &C, math::adder<double> &xadd) const
        {
            xadd.ldz();
            for(const actor *a=crew.head;a;a=a->next)
            {
                xadd.push( C[***a] * a->nu );
            }
            return xadd.get();
        }

        bool conservation_law:: regulate(writable<double>       &target,
                                           const readable<double> &source,
                                           adder_type             &xadd) const
        {
            assert(nrm2>0);
            const double xs = evaluate(source,xadd);
            if(xs<0)
            {
                xadd.ldz();
                iota::load(target,source);
                for(const actor *a=crew.head;a;a=a->next)
                {
                    const double  d = (-xs*a->nu)/nrm2;
                    target[***a] += d;
                    xadd.push(d);
                }
                //std::cerr << xadd << std::endl;
                assert(xadd.size()==crew.size);
                return true;
            }
            else
            {
                return false;
            }
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
