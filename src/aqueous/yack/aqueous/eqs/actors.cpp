#include "yack/aqueous/eqs/actors.hpp"

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


        void actors:: mass_action(const readable<double> &C,
                                  const index_level       I,
                                  cameo::mul<double>     &xmul) const
        {
            for(const actor *a=head;a;a=a->next)
                a->mass_action(I,C,xmul);
        }


        void actors:: mass_action(const readable<double> &C,
                                  const index_level       I,
                                  const double            xi,
                                  cameo::mul<double>     &xmul) const
        {
            for(const actor *a=head;a;a=a->next)
            {
                const species &sp = **a;
                const double   nu = a->nu;
                const double   c  = max_of<double>(C[sp.indx[I]]+(nu*xi),0.0);
                xmul.push(c,nu);
            }
        }



    }

}

