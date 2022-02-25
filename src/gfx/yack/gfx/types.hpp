
//! \file

#ifndef YACK_GFX_TYPES_INCLUDED
#define YACK_GFX_TYPES_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/memory/operative.hpp"
#include "yack/concurrent/tess2d.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    namespace graphic
    {

        class zero_flux
        {
        public:
            zero_flux(const unit_t n) throw();
            ~zero_flux() throw();

            unit_t operator()(unit_t indx) const throw();

            const unit_t size;
            const unit_t szsz;

        private:
            YACK_DISABLE_ASSIGN(zero_flux);
        };

        namespace nexus
        {
            struct bitmem
            {
                static void *acquire(size_t &block_size);
                static void  release(void * &block_addr, size_t &block_size) throw();

            };

            class bitfield : public object, public counted
            {
            public:
                typedef memory::operative io_type;

                explicit bitfield(const size_t n); //!< bytes
                virtual ~bitfield() throw();

                size_t   bytes;
                void    *entry;
                io_type *memio;

                template <typename T>
                void fill(const size_t items) throw()
                {
                    assert(NULL==memio);
                    memio = new( YACK_STATIC_ZSET(wksp) ) memory::operative_of<T>(entry,items);
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(bitfield);
                typedef memory::operative_of<uint8_t> io_type_;
                void *wksp[ YACK_WORDS_FOR(io_type_) ];
            };

            class bitrow
            {
            public:
                bitrow(void *entry, const unit_t width, const zero_flux &zfluz) throw();
                ~bitrow() throw();
                
                void            *p;
                const unit_t     w;
                const zero_flux &z;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(bitrow);
            };



        }


        class metrics
        {
        public:
            //! W>0, H>0, D>0
            metrics(const unit_t W, const unit_t H, const unit_t D) throw();
            metrics(const metrics &) throw();
            virtual ~metrics() throw();

            const unit_t w; //!< width
            const unit_t h; //!< height
            const unit_t n; //!< w*h
            const unit_t d; //!< depth
            const unit_t s; //!< stride = w * depth

        private:
            YACK_DISABLE_ASSIGN(metrics);
        };

        namespace nexus
        {
            class bitrows : public object, public counted
            {
            public:
                explicit bitrows(const metrics &m, void *p);
                virtual ~bitrows() throw();

                const zero_flux zfw;
                const zero_flux zfh;

                bitrow       & operator()(const unit_t y)       throw();
                const bitrow & operator()(const unit_t y) const throw();
                bitrow       & operator[](const unit_t y)       throw();
                const bitrow & operator[](const unit_t y) const throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(bitrows);
                size_t          mem;
                bitrow         *row;
            };
        }

        class bitmap : public metrics
        {
        public:
            virtual ~bitmap() throw() {}

            explicit bitmap(const unit_t W, const unit_t H, const unit_t D) :
            metrics(W,H,D),
            data( new nexus::bitfield(n*d) ),
            rows( new nexus::bitrows(*this,data->entry))
            {

            }

            explicit bitmap(const bitmap &other) throw() :
            metrics(other),
            data(other.data),
            rows(other.rows)
            {
            }

            

        protected:
            arc_ptr<nexus::bitfield> data;
            arc_ptr<nexus::bitrows>  rows;

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

