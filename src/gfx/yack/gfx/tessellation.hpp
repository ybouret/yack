//! \file

#ifndef YACK_GFX_TESSELLATION_INCLUDED
#define YACK_GFX_TESSELLATION_INCLUDED 1

#include "yack/gfx/metrics.hpp"
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
        typedef tess2D::tiles_type          tiles;
        typedef v2d<unit_t>                 coord;  //!< alias

        //______________________________________________________________________
        //
        //
        //! tesselation
        //
        //______________________________________________________________________
        class tessellation : public tess2D
        {
        public:
            //! build tessellation for a metrics and ncpu
            explicit tessellation(const metrics &info,
                                  const size_t   ncpu);

            //! cleanup
            virtual ~tessellation() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tessellation);
        };

    }
}
#endif

