
//! \file

#ifndef YACK_CHEMICAL_GATHERING_INCLUDED
#define YACK_CHEMICAL_GATHERING_INCLUDED 1

#include "yack/chem/entity.hpp"
#include "yack/latch.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! gathering of entities base class
        //
        //______________________________________________________________________
        class gathering : public latch
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~gathering() noexcept; //!< cleanup
            explicit gathering() noexcept; //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void           absorb(const entity &)              noexcept; //!< update maxlen
            std::ostream & pad(std::ostream &os, const entity &) const; //!< pad up to maxlen

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t maxlen; //!< from entities


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gathering);
        };

    }

}

#endif

