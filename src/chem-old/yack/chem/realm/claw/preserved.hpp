//! \file

#ifndef YACK_CHEMICAL_PRESERVED_INCLUDED
#define YACK_CHEMICAL_PRESERVED_INCLUDED 1

#include "yack/chem/realm/claw/conserved.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! base class for preserved
        //
        //______________________________________________________________________
        typedef cxx_list_of<conserved> preserved_;

        //______________________________________________________________________
        //
        //
        //! groups of interlinked conservation laws
        //
        //______________________________________________________________________
        class preserved : public preserved_
        {
        public:
            explicit preserved() noexcept; //!< setup
            virtual ~preserved() noexcept; //!< cleanup
            void     dispatch(const conservation &law); //!< dispatch/create group for law

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(preserved);
        };
    }

}

#endif
