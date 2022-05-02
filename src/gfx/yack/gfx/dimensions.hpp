
//! \file

#ifndef YACK_GFX_DIMS_INCLUDED
#define YACK_GFX_DIMS_INCLUDED 1

#include "yack/setup.hpp"


namespace yack
{
    namespace graphic
    {

        class dimensions
        {
        public:
            virtual ~dimensions() throw();
            dimensions(const unit_t width,
                       const unit_t height) throw();
            dimensions(const dimensions &other) throw();

            const unit_t w; //!< width
            const unit_t h; //!< height

        private:
            YACK_DISABLE_ASSIGN(dimensions);
            };

    }

}

#endif

