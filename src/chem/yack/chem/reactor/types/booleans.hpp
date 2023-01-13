//! \file

#ifndef YACK_CHEMICAL_BOOLEANS_INCLUDED
#define YACK_CHEMICAL_BOOLEANS_INCLUDED 1

#include "yack/object.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/ptr/auto.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //! arrays of booleans
        //______________________________________________________________________
        class booleans : public object, public cxx_array<bool>
        {
        public:

            typedef  auto_ptr<const booleans> ptr;       //!< alias
            explicit booleans(const size_t, const bool); //!< setup
            virtual ~booleans() throw();                 //!< cleanup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(booleans);
        };


    }

}

#endif

