//! \file

#ifndef YACK_CHEMICAL_BOOLEANS_INCLUDED
#define YACK_CHEMICAL_BOOLEANS_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/ptr/arc.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //! arrays of booleans
        //______________________________________________________________________
        class booleans : public object, public counted, public cxx_array<bool>
        {
        public:

            typedef  arc_ptr<const booleans> ptr;       //!< alias
            explicit booleans(const size_t, const bool); //!< setup
            virtual ~booleans() throw();                 //!< cleanup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(booleans);
        };


    }

}

#endif

