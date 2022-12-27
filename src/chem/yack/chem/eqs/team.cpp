#include "yack/chem/eqs/team.hpp"
#include "yack/data/list/sort.hpp"


namespace yack
{
    namespace chemical
    {
        eq_team:: ~eq_team() throw()
        {
        }

        eq_team:: eq_team() throw() : object(), eq_team_()
        {
        }

        eq_team:: eq_team(const eq_team &other) : object(), eq_team_(other) {}

        int eq_team:: compare(const eq_node *lhs, const eq_node *rhs) throw()
        {
            return comparison::increasing(***lhs,***rhs);
        }

        void eq_team:: sort() throw()
        {
            merge_list_of<eq_node>::sort(*this,compare);
        }

    }

}
