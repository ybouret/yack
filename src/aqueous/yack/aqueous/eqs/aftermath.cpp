
#include "yack/aqueous/eqs/aftermath.hpp"

namespace yack
{
    namespace aqueous
    {
        aftermath:: aftermath() noexcept :
        flag(are_blocked),
        xi(0)
        {
        }

        aftermath:: ~aftermath() noexcept
        {
        }

        aftermath:: aftermath(const aftermath &am) noexcept :
        flag(am.flag),
        xi(am.xi)
        {
        }

        aftermath:: aftermath(const double x)      noexcept :
        flag(are_running),
        xi(x)
        {

        }
    }

}

namespace yack
{
    namespace aqueous
    {
        aftermath aftermath:: solve(const equilibrium      &E,
                                      const double            K,
                                      const readable<double> &C0,
                                      writable<double>       &Cs,
                                      xlimits                &xlim,
                                      cameo::mul<double>     &xmul)
        {
            switch(xlim(E,C0))
            {
                    
            }

            return aftermath();
        }

    }

}

