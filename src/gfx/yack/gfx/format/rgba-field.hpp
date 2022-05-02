//! \file

#ifndef YACK_GFX_FORMAT_RGBA_FIELD_INCLUDED
#define YACK_GFX_FORMAT_RGBA_FIELD_INCLUDED 1

#include "yack/gfx/rgb.hpp"
#include "yack/gfx/metrics.hpp"
#include "yack/gfx/article.hpp"
#include "yack/object.hpp"

namespace yack
{
    namespace graphic
    {


        class rgba_field : public article, public metrics
        {
        public:
            virtual ~rgba_field() throw();
            explicit rgba_field(const unit_t width,
                                const unit_t height);
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rgba_field);
            size_t items;
            rgba  *color;
        };

    }
}

#endif
