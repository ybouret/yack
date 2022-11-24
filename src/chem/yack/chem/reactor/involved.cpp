
#include "yack/chem/reactor/involved.hpp"

namespace yack
{

    namespace chemical
    {

        islot::  islot() throw() : islot_() {}
        islot:: ~islot() throw() {}
        islot:: islot(const islot &other) : islot_(other) {}

        std::ostream & operator<<(std::ostream &os, const islot &self)
        {
            const islot::node_type *node = self.head;
            if(node)
            {
                os << "{ " << (**node).name;
                for(node=node->next;node;node=node->next)
                {
                    os << ", " << (**node).name;
                }
                os << " }";
            }
            else
            {
                os << "{}";
            }
            return os;
        }

    }

}
