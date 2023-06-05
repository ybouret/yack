
//! \file

#ifndef YACK_CHEM_VAT_REGULATING_INCLUDED
#define YACK_CHEM_VAT_REGULATING_INCLUDED 1

#include "yack/chem/vat/limit.hpp"


namespace yack
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //! Regulating: Limiting with selected extent
        //
        //______________________________________________________________________
        class Regulating : public Limit
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Regulating(const Species::Fund &) noexcept; //!< setup
            virtual ~Regulating() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! try decrease current value with a new concentration/actor
            void     tryDecreaseWith(const double c, const Actor &a);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Regulating);
        };

    }

}

#endif
