
//! \file

#ifndef YACK_GFX_PIXMAPS_INCLUDED
#define YACK_GFX_PIXMAPS_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/memory.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! pixmaps of given type
        //
        //______________________________________________________________________
        template <typename T>
        class pixmaps : public metrics, public cxx_array< pixmap<T>, memory_allocator >
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef cxx_array< pixmap<T>, memory_allocator > self_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! constructor
            inline explicit pixmaps(const unit_t W,
                                    const unit_t H,
                                    const size_t n) :
            metrics(W,H,sizeof(T)),
            self_type(n, static_cast<const dimensions &>(*this) )
            {
            }

            //! constructor
            inline explicit pixmaps(const dimensions &dims,
                                    const size_t      n) :
            metrics(dims,sizeof(T)),
            self_type(n,static_cast<const dimensions &>(*this) )
            {
            }

            //! cleanup
            inline virtual ~pixmaps() throw() {}
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pixmaps);
        };

    }

}

#endif
