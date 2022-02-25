
//! \file

#ifndef YACK_GFX_TYPES_INCLUDED
#define YACK_GFX_TYPES_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/memory/operative.hpp"
#include "yack/concurrent/tess2d.hpp"
#include "yack/type/args.hpp"
#include "yack/gfx/zero-flux.hpp"
#include "yack/gfx/bitfield.hpp"
#include "yack/gfx/metrics.hpp"
#include "yack/gfx/bitrows.hpp"


namespace yack
{
    namespace graphic
    {




        class bitmap : public metrics
        {
        public:
            virtual ~bitmap() throw() {}

            explicit bitmap(const unit_t W, const unit_t H, const unit_t D) :
            metrics(W,H,D),
            data( new bitfield(n*d) ),
            rows( new bitrows(*this,data->entry))
            {

            }

            explicit bitmap(const bitmap &other) throw() :
            metrics(other),
            data(other.data),
            rows(other.rows)
            {
            }

            

        protected:
            arc_ptr<bitfield> data;
            arc_ptr<bitrows>  rows;

        private:
            YACK_DISABLE_ASSIGN(bitmap);
        };


        template <typename T>
        class pixrow
        {
        public:
            YACK_DECL_ARGS_(T,type);

            inline type & operator()(const unit_t x) throw() {
                assert(x>=0); assert(x<w); assert(0!=p);
                return p[x];
            }

            inline const_type & operator()(const unit_t x) const throw() {
                assert(x>=0); assert(x<w); assert(0!=p);
                return p[x];
            }

            inline type & operator[](const unit_t x) throw() {
                return p[ z(x) ];
            }

            inline const_type & operator[](const unit_t x) const throw() {
                return p[ z(x) ];
            }


        private:
            T               *p;

        public:
            const unit_t     w;
            const zero_flux &z;

        private:
            pixrow() throw();
            ~pixrow();
            YACK_DISABLE_COPY_AND_ASSIGN(pixrow);
        };

        template <typename T>
        class pixmap : public bitmap
        {
        public:
            typedef pixrow<T> row_type;

            inline explicit pixmap(const unit_t W,
                                   const unit_t H) :
            bitmap(W,H,sizeof(T))
            {
                data->fill<T>(n);
            }

            inline virtual ~pixmap() throw() {}

            inline row_type & operator()(const unit_t y) throw()
            {
                return *(row_type *) & (*rows)(y);
            }

            inline const row_type & operator()(const unit_t y) const throw()
            {
                return *(const row_type *) & (*rows)(y);
            }

            inline row_type & operator[](const unit_t y) throw()
            {
                return *(row_type *) & (*rows)[y];
            }

            inline const row_type & operator[](const unit_t y) const throw()
            {
                return  *(row_type *) & (*rows)[y];
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pixmap);

        };


    }

}

#endif

