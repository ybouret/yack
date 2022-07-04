
#include "yack/chem/mix/groups.hpp"

namespace yack
{
    namespace chemical
    {
        const groups::build_attached_ groups::build_attached = {};

        groups:: groups(const equilibria &eqs, const build_attached_ &) : group::list()
        {
            groups &G = *this;

            //------------------------------------------------------------------
            //
            // first pass: create groups
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                bool               found = false;
                for(group *g= G.head;g;g=g->next)
                {
                    if(g->attached(eq))
                    {
                        (*g) << &eq;
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    G.push_back( group::new_from(eq) );
                }
            }
            
            //------------------------------------------------------------------
            //
            // full sort
            //
            //------------------------------------------------------------------
            sort();

            //------------------------------------------------------------------
            //
            // set partition info
            //
            //------------------------------------------------------------------
            size_t info = 1;
            for(group *g=head;g;g=g->next,++info)
            {
                for(gnode *node=g->head;node;node=node->next)
                {
                    coerce((**node).info) = info;
                }
            }


        }

    }

}

