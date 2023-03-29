#include "yack/chem/eqs/boundaries.hpp"

namespace yack
{
    namespace chemical
    {
        
        boundaries:: ~boundaries() noexcept
        {

        }

        boundaries:: boundaries(const zl_proxy &zlp,
                                const sp_proxy &spp) noexcept :
        zlimits(zlp),
        spore(spp)
        {
        }


        bool boundaries:: check() const noexcept
        {
            if(size>=2)
            {
                for(const zl_node *node=head, *next=head->next;next;node=next,next=next->next)
                {
                    if( (**node).extent >= (**next).extent ) return false;
                }
            }
            return true;
        }

       
      


    }
}
