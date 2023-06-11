
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
            explicit ConstEquilibrium(const NAME &id, const double K0) :
            Equilibrium(id),
            K_(K0) {}
            virtual ~ConstEquilibrium() noexcept; //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
            const Extended::Real K_;
            virtual Extended::Real getK(const double);
        };
    }

}

#endif

