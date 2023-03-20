
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
        regular(spp),
        invalid(zlp,spp)
        {

        }

        void frontiers:: reopen() noexcept
        {
            regular.reset();
            invalid.clear();
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
                    regular.insert(c/a->nu,sp);
                }
                else
                {
                    invalid.insert(c/a->nu,sp);
                }

            }
        }

        std::ostream & operator<<(std::ostream &os, const frontiers &self)
        {
            os << "regular: " << self.regular << " | invalid: " << self.invalid;
            return os;
        }

    }

}

