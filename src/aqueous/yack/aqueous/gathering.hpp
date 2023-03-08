//! \file

#ifndef YACK_AQUEOUS_GATHERING_INCLUDED
#define YACK_AQUEOUS_GATHERING_INCLUDED 1

#include "yack/aqueous/entity.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! base class for gatherings: max name length of its enities
        //
        //______________________________________________________________________
        class gathering
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~gathering()         noexcept; //!< cleanup
            explicit gathering()         noexcept; //!< setup length=0
            gathering(const gathering &) noexcept; //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void           update(const entity &)           noexcept; //!< updated length
            std::ostream & pad(std::ostream &, const entity &) const; //!< pad w.r.t name.size()

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t length; //!< max name length

        private:
            YACK_DISABLE_ASSIGN(gathering);
        };
    }

}

#endif

