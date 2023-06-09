
//! \file

#ifndef YACK_CHEM_VAT_TRIBES_INCLUDED
#define YACK_CHEM_VAT_TRIBES_INCLUDED 1

#include "yack/chem/vat/cluster/tribe.hpp"

namespace yack
{
    namespace Chemical
    {

        typedef cxx_list_of<Tribe> Tribes_; //!< alias

        //______________________________________________________________________
        //
        //
        //! List of Tribes to regulate or equalize
        //
        //______________________________________________________________________
        class Tribes : public Tribes_
        {
        public:
            explicit Tribes() noexcept; //!< setup
            virtual ~Tribes() noexcept; //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Tribes);
        };
    }

}

#endif

