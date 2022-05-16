//! \file

#ifndef YACK_GFX_PIXMAP_INCLUDED
#define YACK_GFX_PIXMAP_INCLUDED 1

#include "yack/gfx/pixrow.hpp"
#include "yack/gfx/bitmap.hpp"
#include "yack/gfx/broker/transform.hpp"

namespace yack
{
    namespace graphic
    {

        //! helper for constructors
#define YACK_GFX_PIXMAP_CTOR()           \
row( coerce_cast<row_type>(rows->row) ), \
zfh(rows->zfh)

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

            //! create by WxH
            inline explicit pixmap(const unit_t W,
                                   const unit_t H) :
            bitmap(W,H,sizeof(T)),
            YACK_GFX_PIXMAP_CTOR()
            {
                data->fill<T>(n);
            }

            //! create by dimensions
            inline explicit pixmap(const dimensions &dims) :
            bitmap(dims.w,dims.h,sizeof(T)),
            YACK_GFX_PIXMAP_CTOR()
            {
                data->fill<T>(n);
            }

            //! shared copy
            inline pixmap(const pixmap &other) throw() :
            bitmap(other),
            YACK_GFX_PIXMAP_CTOR()
            {
            }

            //! hard copy
            template <typename U,typename PROC>
            inline pixmap(const pixmap<U> &source,
                          broker          &device,
                          PROC            &U_to_T) :
            bitmap(source.w,source.h,sizeof(T)),
            YACK_GFX_PIXMAP_CTOR()
            {
                // initialize
                data->fill<T>(n);

                // transform
                broker_transform::apply(*this,source,device,U_to_T);
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

            //! direct access
            inline T & operator()(const coord pos) throw()
            {
                return (*this)(pos.y)(pos.x);
            }

            //! direct CONST access
            inline const T & operator()(const coord pos) const throw()
            {
                return (*this)(pos.y)(pos.x);
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
