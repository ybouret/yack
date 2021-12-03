
#include "yack/jive/pattern/logic/bank.hpp"

namespace yack
{
    namespace jive
    {

        bank:: ~bank() throw()
        {
        }

        bank:: bank(const uint32_t t) throw() : logical(t)
        {

        }

        void bank:: no_multiple() throw()
        {
            patterns temp;
            while(size)
            {
                bool found = false;
                for(const pattern *m=temp.head;m;m=m->next)
                {
                    if( *m == *head )
                    {
                        found = true;
                        break;
                    }
                }
                pattern *mine = pop_front();
                if(found)
                {
                    delete mine;
                }
                else
                {
                    temp.push_back(mine);
                }
            }
            swap_with(temp);
        }

    }

}

