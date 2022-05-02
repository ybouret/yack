
//! \file

#ifndef YACK_GFX_PIXMAP_INCLUDED
#define YACK_GFX_PIXMAP_INCLUDED 1

#include "yack/gfx/pixrow.hpp"
#include "yack/gfx/bitmap.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! pixmap of given type
        //
        //______________________________________________________________________
        template <typename T>
        class pixmap : public bitmap
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef pixrow<T> row_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! create
            inline explicit pixmap(const unit_t W,
                                   const unit_t H) :
            bitmap(W,H,sizeof(T)),
            row( coerce_cast<row_type>(rows->row) ),
            zfh(rows->zfh)
            {
                data->fill<T>(n);
            }

            //! shared copy
            inline pixmap(const pixmap &other) throw() :
            bitmap(other),
            row(coerce_cast<row_type>(rows->row) ),
            zfh(rows->zfh)
            {
            }

            //! cleanup
            inline virtual ~pixmap() throw() {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! direct access
            inline row_type & operator()(const unit_t y) throw()
            {
                assert(y>=0); assert(y<h);
                return row[y];
            }

            //! direct CONST access
            inline const row_type & operator()(const unit_t y) const throw()
            {
                assert(y>=0); assert(y<h);
                return row[y];
            }

            //! zero-flux access
            inline row_type & operator[](const unit_t y) throw()
            {
                return row[zfh(y)];
            }

            //! zero-flux CONST access
            inline const row_type & operator[](const unit_t y) const throw()
            {
                return  row[zfh(y)];
            }

        private:
            YACK_DISABLE_ASSIGN(pixmap);
            row_type       *row;
            const zero_flux zfh;
        };

    }

}

#endif
