#include "yack/aqueous/actors.hpp"

namespace yack
{
    namespace aqueous
    {
        actors:: ~actors() noexcept
        {

        }

        actors:: actors() noexcept : object(), actors_(), next(0)
        {
        }

        actors:: actors(const actors &other) : object(), actors_(other), next(0)
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

        string actors:: to_string() const
        {
            const actor *node = head;
            if(node)
            {
                string res = node->to_string_first();
                for(node=node->next;node;node=node->next)
                    res += node->to_string_extra();
                return res;
            }
            else
            {
                return '.';
            }
        }

    }

}

