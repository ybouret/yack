
//! \file

#ifndef YACK_CHEM_CONST_EQUILIBRIUM_INCLUDED
#define YACK_CHEM_CONST_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! constant equilibrium
        //
        //______________________________________________________________________
        class ConstEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename NAME> inline
            explicit ConstEquilibrium(const NAME &id) : Equilibrium(id) {}
            virtual ~ConstEquilibrium() noexcept; //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
        };
    }

}

#endif

