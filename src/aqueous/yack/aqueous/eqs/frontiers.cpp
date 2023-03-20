
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

        void frontiers:: reopen() noexcept
        {
            lim.reset();
            oor.clear();
        }

        void frontiers:: detect(const actors           &A,
                                const readable<double> &C,
                                const readable<bool>   &R)
        {
            reopen();
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
                    oor.insert(c/a->nu,sp);
                }

            }
        }

        std::ostream & operator<<(std::ostream &os, const frontiers &self)
        {

            static const char none [] = "none";
            os << "lim: ";
            if(self.lim.size)
            {
                os << self.lim;
            }
            else
            {
                os << none;
            }
            os << " | oor: ";
            if(self.oor.size)
            {
                os << self.oor;
            }
            else
            {
                os << none;
            }
            return os;
        }

    }

}

