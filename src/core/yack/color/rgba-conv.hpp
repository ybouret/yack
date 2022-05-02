//! \file

#ifndef YACK_COLOR_RGBA_CONV_INCLUDED
#define YACK_COLOR_RGBA_CONV_INCLUDED 1

#include "yack/color/rgba.hpp"

namespace yack
{
    namespace color
    {

        class rgba_conv  
        {
        public:
            virtual ~rgba_conv() throw();
            const unit_t d; //!< depth

        protected:
            explicit rgba_conv(const unit_t depth) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rgba_conv);
        };

    }

}

#endif
