
#include "yack/aqueous/eqs/aftermath.hpp"

namespace yack
{
    namespace aqueous
    {
        aftermath:: aftermath() noexcept :
        state(are_blocked),
        value(0)
        {
        }

        aftermath:: ~aftermath() noexcept
        {
        }

        aftermath:: aftermath(const aftermath &am) noexcept :
        state(am.state),
        value(am.value)
        {
        }

        aftermath:: aftermath(const double x)      noexcept :
        state(are_running),
        value(x)
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
                                    cameo::mul<double>     &xmul,
                                    cameo::add<double>     &xadd)
        {
            switch(xlim(E,C0))
            {
                    
            }

            return aftermath();
        }

    }

}

