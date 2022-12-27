
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


namespace yack
{
    namespace chemical
    {

        eq_squads::  eq_squads() throw() :  cxx_list_of<eq_squad>() {}
        eq_squads:: ~eq_squads() throw() {}

    }

}

