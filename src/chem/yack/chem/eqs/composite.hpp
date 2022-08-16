

//! \file

#ifndef YACK_CHEMICAL_EQS_COMPOSITE_INCLUDED
#define YACK_CHEMICAL_EQS_COMPOSITE_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/container/tuple/pair.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! composite equilibrium management
        //
        //______________________________________________________________________
        struct composite
        {
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! coefficient
            //__________________________________________________________________
            YACK_PAIR_DECL(STANDARD,coeff,int,lhs,int,rhs);
            coeff() throw(); //!< default setup
            void normalize() throw(); //!< order
            YACK_PAIR_END(coeff); //!< end of coeff

            //__________________________________________________________________
            //
            //! coefficients
            //_________________________________________________________________
            typedef vector<coeff> coeffs;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! collect all possible shared coefficients
            /**
             \param cof weights to combine lhs and lhs to
             \param lhs set of components
             \param lhs set of components
             */
            static size_t collect(coeffs           &cof,
                                  const components &lhs,
                                  const components &rhs);


            static void scatter(equilibria             &couples,
                                library                &libcopy,
                                const equilibria       &singles,
                                const readable<double> &K,
                                rmulops                &xmul);

            static double getK(const coeff &c, const double lhsK, const double rhsK, rmulops &xmul);

        };

    }

}

#endif
