
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

            unit_t operator[](unit_t indx) const throw();


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

                explicit bitfield(const size_t n);
                virtual ~bitfield() throw();

                size_t bytes;
                void  *entry;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(bitfield);
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

        


    }
}

#endif

