
#include "yack/chem/nexus/cluster.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {

        cluster:: cluster() :
        eq_team(),
        next(0),
        prev(0),
        working(),
        canon( new conservation_laws() ),
        cells( new claw_teams  ),
        roaming( new eq_team() ),
        bounded( new eq_team() )
        {}
        cluster::~cluster() throw() {}

        bool cluster:: attached_to(const equilibrium &eq) const throw()
        {
            for(const eq_node *node=head;node;node=node->next)
            {
                const equilibrium &mine = **node;
                if(mine.attached_to(eq))
                    return true;
            }
            return false;
        }

        bool cluster:: attached_to(const cluster &rhs) const throw()
        {
            for(const eq_node *node=head;node;node=node->next)
            {
                const equilibrium &mine = **node;
                if(rhs.attached_to(mine)) return true;
            }
            return false;
        }


        static inline int eq_node_compare(const eq_node *lhs,
                                          const eq_node *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            return comparison::increasing(***lhs,***rhs);
        }

        void cluster:: sort()
        {
            active_list &A = coerce(working);
            A.release();
            merge_list_of<eq_node>::sort(*this,eq_node_compare);
            {
                addrbook tribe;
                for(const eq_node *en=head;en;en=en->next)
                {
                    (**en).update(tribe);
                }
                for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                {
                    A << static_cast<const species *>(*it);
                }
            }
            merge_list_of<anode>::sort(A,sp_list::node_compare);
        }

    }

}

namespace yack
{
    namespace chemical
    {

        clusters::  clusters() throw() : clusters_() {}
        clusters:: ~clusters() throw() {}


    }

}
