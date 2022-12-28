
#include "yack/chem/eqs/squad.hpp"

namespace yack
{
    namespace chemical
    {

        eq_squad::  eq_squad() throw() :  eq_team(), next(0), prev(0) {}
        eq_squad:: ~eq_squad() throw() {}
        eq_squad::  eq_squad(const eq_squad &other) :  eq_team(other), next(0), prev(0) {}

        eq_squad * eq_squad:: clone( ) const
        {
            return new eq_squad( *this );
        }

        int eq_squad:: compare(const eq_squad *lhs, const eq_squad *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            if(lhs->size<rhs->size) {
                return -1;
            }
            else
            {
                if(rhs->size<lhs->size)
                {
                    return 1;
                }
                else
                {
                    for(const eq_node *L=lhs->head, *R=rhs->head;L;L=L->next,R=R->next)
                    {
                        const size_t il = ***L;
                        const size_t ir = ***R;
                        switch( __sign::of(il,ir) )
                        {
                            case negative: return -1;
                            case __zero__: continue;
                            case positive: return 1;
                        }
                    }
                    return 0;
                }
            }
        }

    }

}

#include "yack/ios/xmlog.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {

        eq_squads::  eq_squads() throw() :  cxx_list_of<eq_squad>() {}
        eq_squads:: ~eq_squads() throw() {}


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

        void eq_squads:: build(const eq_team      &guests,
                               const matrix<bool> &detached,
                               const xmlog        &xml)
        {
            static const char * const here = "equilibria::squads::build";
            YACK_XMLSUB(xml,here);
            assert(0==size);

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            for(const eq_node *node=guests.head;node;node=node->next)
            {
                const equilibrium    &lhs = **node;
                push_back( new eq_squad() )->append(&lhs);
            }

            //------------------------------------------------------------------
            //
            // for each existing squad, to grow it
            // with a new equilibrium, totally detached
            //
            //------------------------------------------------------------------
            for(const eq_node *node=guests.head;node;node=node->next)
            {
                const equilibrium &rhs = **node;
                const size_t       rid = *rhs;
                for(eq_squad *squad=head;squad;squad=squad->next)
                {
                    assert(squad->tail);
                    if(***(squad->tail)>=rid)                  continue; // keep for later
                    if(!is_detached_from(*squad,rhs,detached)) continue; // not possible
                    push_front( squad->clone() )->append(&rhs);          // clone with new
                }
            }

            //------------------------------------------------------------------
            //
            // cleanup tree
            //
            //------------------------------------------------------------------
            assert(size>=guests.size);
            YACK_XMLOG(xml, "-- sorting " << size << " / " << guests.size << " : added #" << size - guests.size);

            for(eq_squad *squad=head;squad;squad=squad->next) squad->sort();
            merge_list_of<eq_squad>::sort(*this,eq_squad::compare);

            for(const eq_squad *squad=head;squad;squad=squad->next)
            {

                if( !independent(*squad,detached) ) {
                    YACK_XMLOG(xml,"-- invalid " << *squad);
                    throw imported::exception(here,"unexpected invalid equilibria squad");
                }

                if(squad->size>1) {
                    YACK_XMLOG(xml,"  (+) " << *squad);
                }
            }



        }

    }

}

