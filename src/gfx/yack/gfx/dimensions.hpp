
//! \file

#ifndef YACK_GFX_DIMS_INCLUDED
#define YACK_GFX_DIMS_INCLUDED 1

#include "yack/gfx/coord.hpp"


namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! 2D dimensions
        //
        //______________________________________________________________________
        class dimensions
        {
        public:
            virtual ~dimensions() noexcept;                      //!< cleanup
            dimensions(const unit_t W, const unit_t H) noexcept; //!< setup
            dimensions(const dimensions &other) noexcept;        //!< copy


            bool matches_dimensions(const dimensions &)   const noexcept; //!< testing
            bool contains(const unit_t i, const unit_t j) const noexcept; //!< testing
            bool contains(const coord &)                  const noexcept; //!< testing

            const unit_t w; //!< width
            const unit_t h; //!< height
            

        private:
            YACK_DISABLE_ASSIGN(dimensions);
        };

    }

}

#endif

