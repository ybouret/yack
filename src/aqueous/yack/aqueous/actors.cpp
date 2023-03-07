#include "yack/aqueous/actors.hpp"

namespace yack
{
    namespace aqueous
    {
        actors:: ~actors() noexcept
        {

        }

        actors:: actors() noexcept : actors_()
        {
        }

        actors:: actors(const actors &other) : actors_(other)
        {
        }

        std::ostream & operator<<(std::ostream &os, const actors &self)
        {
            const actor *node = self.head;
            if(node)
            {
                node->display_first(os);
                for(node=node->next;node;node=node->next)
                    node->display_extra(os);
            }
            else
            {
                os << '.';
            }
            return os;
        }


    }

}
