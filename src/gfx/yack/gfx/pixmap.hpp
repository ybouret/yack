
//! \file

#ifndef YACK_GFX_PIXMAP_INCLUDED
#define YACK_GFX_PIXMAP_INCLUDED 1

#include "yack/gfx/pixrow.hpp"
#include "yack/gfx/bitmap.hpp"
#include "yack/gfx/broker.hpp"

namespace yack
{
    namespace graphic
    {
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

            //! create
            inline explicit pixmap(const unit_t W,
                                   const unit_t H) :
            bitmap(W,H,sizeof(T)),
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

                struct task
                {
                    pixmap<T>       &target;
                    const pixmap<U> &source;
                    PROC            &U_to_T;
                    static inline
                    void make(void *args,const tiles &t, const context &, lockable &) throw()
                    {
                        task            &self   = *static_cast<task *>(args);
                        pixmap<T>       &target = self.target;
                        const pixmap<U> &source = self.source;
                        PROC            &U_to_T = self.U_to_T;
                        for(const tile *node=t.head();node;node=node->next)
                        {
                            size_t           len = node->width;
                            coord            pos = node->start;
                            pixrow<T>       &tgt = target(pos.y);
                            const pixrow<U> &src = source(pos.y);
                            while(len-- > 0 )
                            {
                                tgt(pos.x) = U_to_T(src(pos.x));
                                ++pos.x;
                            }
                        }
                    }
                };

                task todo = { *this, source, U_to_T };
                device(task::make,&todo);

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
