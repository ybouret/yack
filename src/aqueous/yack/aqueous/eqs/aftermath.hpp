//! \file

#ifndef YACK_AQUEOUS_AFTERMATH_INCLUDED
#define YACK_AQUEOUS_AFTERMATH_INCLUDED 1

#include "yack/aqueous/eqs/xlimits.hpp"
#include "yack/aqueous/equilibrium.hpp"

namespace yack
{
    namespace aqueous
    {

        class aftermath
        {
        public:
            aftermath()                  noexcept;
            aftermath(const aftermath &) noexcept;
            ~aftermath()                 noexcept;
            aftermath(const double)      noexcept;

            const availability flag;
            const double       xi;

            static aftermath solve(const equilibrium      &E,
                                   const double            K,
                                   const readable<double> &C0,
                                   writable<double>       &Cs,
                                   xlimits                &xlim,
                                   cameo::mul<double>     &xmul);


        };
        
    }

}

#endif

