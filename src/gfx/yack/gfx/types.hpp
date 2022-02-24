
//! \file

#ifndef YACK_GFX_TYPES_INCLUDED
#define YACK_GFX_TYPES_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/memory/operative.hpp"
#include "yack/concurrent/tess2d.hpp"

namespace yack
{
    namespace graphic
    {

        class zero_flux
        {
        public:
            zero_flux(const unit_t n) throw();
            ~zero_flux() throw();

            unit_t get(unit_t indx) const throw();

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
                void fill(const size_t n) throw()
                {
                    assert(NULL==memio);
                    memio = new( YACK_STATIC_ZSET(wksp) ) memory::operative_of<T>(entry,n);
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
                size_t          mem;
                bitrow         *row;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(bitrows);

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
            

        protected:
            arc_ptr<nexus::bitfield> data;
            arc_ptr<nexus::bitrows>  rows;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bitmap);
        };

        template <typename T>
        class pixmap : public bitmap
        {
        public:

            inline explicit pixmap(const unit_t W,
                                   const unit_t H) :
            bitmap(W,H,sizeof(T))
            {
                data->fill<T>(n);
            }

            inline virtual ~pixmap() throw() {}


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pixmap);

        };


    }

}

#endif

