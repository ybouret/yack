
//! \file

#ifndef YACK_GFX_PIXMAPS_INCLUDED
#define YACK_GFX_PIXMAPS_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/memory.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T>
        class pixmaps : public metrics, public cxx_array< pixmap<T>, memory_allocator >
        {
        public:
            typedef cxx_array< pixmap<T>, memory_allocator > self_type;

            inline explicit pixmaps(const unit_t W,
                                    const unit_t H,
                                    const size_t n) :
            metrics(W,H,sizeof(T)),
            self_type(n, static_cast<const dimensions &>(*this) )
            {
            }

            inline virtual ~pixmaps() throw()
            {
            }
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pixmaps);
        };

    }

}

#endif
