//! \file

#ifndef YACK_AQUEOUS_AFTERMATH_INCLUDED
#define YACK_AQUEOUS_AFTERMATH_INCLUDED 1

#include "yack/aqueous/eqs/xlimits.hpp"
#include "yack/aqueous/equilibrium.hpp"
#include "yack/cameo/add.hpp"

namespace yack
{
    namespace aqueous
    {

        //______________________________________________________________________
        //
        //
        //! solving ONE equilibrium
        //
        //______________________________________________________________________
        class aftermath
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            aftermath()                                noexcept; //!< blocked,0
            aftermath(const aftermath &)               noexcept; //!< copy
            ~aftermath()                               noexcept; //!< cleanup
            aftermath(const double x)                  noexcept; //!< running, x
            aftermath(const double x, const double dx) noexcept; //!< running, x, error

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const availability state; //!< current state
            const double       value; //!< current value
            const double       error; //!< numerical mass action

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            YACK_PROTO_OSTREAM(aftermath);

            //! solving algorithm
            static aftermath solve(const equilibrium      &E,
                                   const double            K,
                                   const readable<double> &C0,
                                   writable<double>       &Cs,
                                   const index_level       il,
                                   xlimits                &xlim,
                                   cameo::mul<double>     &xmul,
                                   cameo::add<double>     &xadd);

        private:
            YACK_DISABLE_ASSIGN(aftermath);

        };
        
    }

}

#endif

