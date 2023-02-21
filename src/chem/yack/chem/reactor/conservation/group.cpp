#include "yack/chem/reactor/conservation/group.hpp"

namespace yack
{
    namespace chemical
    {

        clot:: clot(const claw &first) :
        cl_repo(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        clot:: ~clot() noexcept
        {
        }

        bool clot:: attached_to(const claw &cl) const noexcept
        {
            for(const node_type *node=head;node;node=node->next)
            {
                const claw &mine = ***node;
                if(mine.is_linked_to(cl)) return true;
            }
            return false;
        }

        bool clot:: attached_to(const clot &g) const noexcept
        {
            assert( &g != this );
            for(const node_type *node=g.head;node;node=node->next)
            {
                const claw &cl = ***node;
                if(attached_to(cl)) return true;
            }
            return false;
        }



    }

}


