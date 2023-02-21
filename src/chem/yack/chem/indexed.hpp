//! \file

#ifndef YACK_CHEMICAL_INDEXED_INCLUDED
#define YACK_CHEMICAL_INDEXED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! indexed type
        //
        //______________________________________________________________________
        class indexed
        {
        public:
            explicit indexed(const size_t) noexcept; //!< with positive index
            virtual ~indexed() noexcept;             //!< cleanup
            indexed(const indexed &) noexcept;       //!< copy
            size_t operator*() const noexcept;       //!< get index

        private:
            const size_t indx_;
            YACK_DISABLE_ASSIGN(indexed);
        };

    }

}

#endif
