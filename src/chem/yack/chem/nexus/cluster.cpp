
#include "yack/chem/nexus/cluster.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {

        cluster:: cluster() :
        eq_team(),
        next(0),
        prev(0),
        working(),
        canon(   new conservation_laws() ),
        cells(   new claw_teams  ),
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


        static inline bool is_detached_from(const eq_squad     &squad,
                                            const equilibrium  &eq,
                                            const matrix<bool> &detached) throw()
        {
            const readable<bool> &det = detached[*eq];
            for(const eq_node *node=squad.head;node;node=node->next)
            {
                if( !det[ ***node] ) return false;
            }
            return true;
        }

        static inline bool independent(const eq_squad     &squad,
                                       const matrix<bool> &detached) throw()
        {
            for(const eq_node *node=squad.head;node;node=node->next)
            {
                const readable<bool> &det = detached[ ***node ];
                for(const eq_node *scan=node->next;scan;scan=scan->next)
                {
                    if( !det[ ***scan] ) return false;
                }
            }
            return true;
        }



        void cluster:: build_squads(eq_squads          &groups,
                                    const eq_team      &guests,
                                    const matrix<bool> &detached,
                                    const xmlog        &xml) 
        {
            static const char * const here = "cluster:build_squads";
            YACK_XMLSUB(xml,here);

            assert(0==groups.size);
            for(const eq_node *node=guests.head;node;node=node->next)
            {
                const equilibrium    &lhs = **node;
                groups.push_back( new eq_squad() )->append(&lhs);
                for(const eq_node *scan=node->next;scan;scan=scan->next)
                {
                    const equilibrium &rhs = **scan;
                    for(eq_squad *squad=groups.head;squad;squad=squad->next)
                    {
                        if( is_detached_from(*squad, rhs, detached) )
                        {
                            groups.push_front( squad->clone() )->append(&rhs);
                            for(const eq_squad *sub=groups.head->next;sub;sub=sub->next)
                            {
                                if(0==eq_squad::compare(groups.head,sub) )
                                {
                                    delete groups.pop_front();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            assert(groups.size>=guests.size);
            YACK_XMLOG(xml, "-- sorting " << groups.size << " / " << guests.size << " : added #" << groups.size - guests.size);
            merge_list_of<eq_squad>::sort(groups,eq_squad::compare);

            for(const eq_squad *squad=groups.head;squad;squad=squad->next)
            {
                if( !independent(*squad,detached) ) {
                    YACK_XMLOG(xml,"-- invalid " << *squad);
                    throw imported::exception(here,"unexpected invalid equilibria squad");
                }
                if(squad->size>1)
                {
                    YACK_XMLOG(xml,"  (+) " << *squad);
                }
            }

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
