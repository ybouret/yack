
#include "yack/chem/realm/claw/preserved.hpp"

namespace yack
{
    namespace chemical
    {
        preserved:: ~preserved() noexcept
        {
        }

        preserved:: preserved() noexcept : preserved_()
        {
        }

        void preserved:: dispatch(const conservation &law)
        {
            bool accepted = false;
            for(conserved *lhs=head;lhs;lhs=lhs->next)
            {
                if(lhs->accepts(law))
                {
                    accepted = true;
                    (*lhs) << law;
                    break;
                }
            }

            if(!accepted)
            {
                // create new group
                push_back( new conserved(law) );
            }
            else
            {
                // reduce
                preserved_ keep;
                while(size>0)
                {
                    conserved *rhs = pop_front();
                    for(conserved *lhs=keep.head;lhs;lhs=lhs->next)
                    {
                        if(lhs->accepts(*rhs))
                        {
                            lhs->merge_back(*rhs);
                            assert(0==rhs->size);
                            delete rhs;
                            rhs = NULL;
                            break;
                        }
                    }
                    if(rhs) keep.push_back(rhs);
                }
                swap_with(keep);
            }


        }

    }

}
