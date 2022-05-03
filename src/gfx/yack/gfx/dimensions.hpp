
//! \file

#ifndef YACK_GFX_DIMS_INCLUDED
#define YACK_GFX_DIMS_INCLUDED 1

#include "yack/setup.hpp"


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
            virtual ~dimensions() throw();                      //!< cleanup
            dimensions(const unit_t W, const unit_t H) throw(); //!< setup
            dimensions(const dimensions &other) throw();        //!< copy

            //! testing
            bool matches_dimensions(const dimensions &) const throw();


            const unit_t w; //!< width
            const unit_t h; //!< height


        private:
            YACK_DISABLE_ASSIGN(dimensions);
        };

    }

}

#endif

