
//! \file

#ifndef YACK_GFX_TYPES_INCLUDED
#define YACK_GFX_TYPES_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/memory/operative.hpp"

namespace yack
{
    namespace graphic
    {
        
        namespace nexus
        {
            struct bitmem
            {
                static void *acquire(size_t &block_size);
                static void  release(void * &block_addr, size_t &block_size) throw();

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

