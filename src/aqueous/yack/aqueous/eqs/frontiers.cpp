
#include "yack/aqueous/eqs/frontiers.hpp"

namespace yack
{
    namespace aqueous
    {

        frontiers:: ~frontiers() noexcept
        {
        }

        frontiers:: frontiers(const zl_proxy &zlp,
                              const sp_proxy &spp) noexcept :
        lim(spp),
        oor(zlp,spp)
        {

        }

        void frontiers:: initialize() noexcept
        {
            lim.reset();
            oor.clear();
        }

        bool frontiers:: honored_by(const actors           &A,
                                    const readable<double> &C,
                                    const readable<bool>   &R)
        {
            initialize();
            for(const actor *a=A.head;a;a=a->next)
            {
                const species &sp = **a;
                const size_t   j  = sp.indx[top_level]; if( !R[j] ) continue;
                const double   c  = C[j];
                if(c>=0)
                {
                    lim.insert(c/a->nu,sp);
                }
                else
                {
                    oor.insert((-c)/a->nu,sp);
                }
            }
            return oor.size<=0;
        }

        std::ostream & operator<<(std::ostream &os, const frontiers &self)
        {

            if(self.lim.size)
            {
                os << "[lim: " << self.lim << "]";
            }

            if(self.oor.size)
            {
                os << "[oor: " << self.oor << "]";
            }

            return os;
        }

    }

}

