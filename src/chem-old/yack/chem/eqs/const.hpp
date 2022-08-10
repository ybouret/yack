//! \file

#ifndef YACK_CHEM_CONST_EQUILIBRIUM_INCLUDED
#define YACK_CHEM_CONST_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! const equilibrium
        //
        //______________________________________________________________________
        class const_equilibrium : public equilibrium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename ID> inline
            const_equilibrium(const ID    &uuid,
                              const size_t ipos,
                              const double kval) :
            equilibrium(uuid,ipos),
            value(kval)
            {
            }

            //! cleanup
            virtual ~const_equilibrium() throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(const_equilibrium);
            const double value;
            virtual double getK(const double) const;
        };

    }

}

#endif
