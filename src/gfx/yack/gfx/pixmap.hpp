
//! \file

#ifndef YACK_GFX_PIXMAP_INCLUDED
#define YACK_GFX_PIXMAP_INCLUDED 1

#include "yack/gfx/pixrow.hpp"
#include "yack/gfx/bitmap.hpp"

namespace yack
{
    namespace graphic
    {
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            typedef pixrow<T> row_type;

            inline explicit pixmap(const unit_t W,
                                   const unit_t H) :
            bitmap(W,H,sizeof(T)),
            row( coerce_cast<row_type>(rows->row) ),
            zfh(rows->zfh)
            {
                data->fill<T>(n);
            }

            inline virtual ~pixmap() throw() {}

            inline row_type & operator()(const unit_t y) throw()
            {
                assert(y>=0); assert(y<h);
                return row[y];
            }

            inline const row_type & operator()(const unit_t y) const throw()
            {
                assert(y>=0); assert(y<h);
                return row[y];
            }

            inline row_type & operator[](const unit_t y) throw()
            {
                return row[zfh(y)];
            }

            inline const row_type & operator[](const unit_t y) const throw()
            {
                return  row[zfh(y)];
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pixmap);
            row_type       *row;
            const zero_flux zfh;
        };

    }

}

#endif
