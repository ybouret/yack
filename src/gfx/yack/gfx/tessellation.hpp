//! \file

#ifndef YACK_GFX_TESSELLATION_INCLUDED
#define YACK_GFX_TESSELLATION_INCLUDED 1

#include "yack/gfx/dimensions.hpp"
#include "yack/gfx/coord.hpp"
#include "yack/concurrent/tess2d.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        // 2D types
        //
        //______________________________________________________________________
        typedef concurrent::tess2D<unit_t>  tess2D; //!< base class for tessellation
        typedef tess2D::tiles_type          tiles;  //!< alias
        typedef tiles::tile_type            tile;   //!< alias

        //______________________________________________________________________
        //
        //
        //! tesselation
        //
        //______________________________________________________________________
        class tessellation : public dimensions, public tess2D
        {
        public:
            //! build tessellation for dimensions and ncpu
            explicit tessellation(const dimensions &dims,
                                  const size_t      ncpu);

            //! cleanup
            virtual ~tessellation() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tessellation);
        };

    }
}
#endif

