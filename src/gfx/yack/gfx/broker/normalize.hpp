
//! \file

#ifndef YACK_GFX_BROKER_NORMALIZE_INCLUDED
#define YACK_GFX_BROKER_NORMALIZE_INCLUDED 1

#include "yack/gfx/broker/transform.hpp"

namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! normalize
        //
        //______________________________________________________________________
        struct broker_normalize
        {
            //! wrapper to call transform
            template <typename T> struct procedure
            {
                T vmin; //!< vmin
                T vmax; //!< vmax
                T scal; //!< vmax-vmin
                void operator()(T &v) const throw(); //!< apply
            };

            //! normalize each pixel with procedure
            template <typename T> static inline
            void apply(pixmap<T> &target, broker &device, const T vmin, const T vmax)
            {
                if(vmax>vmin)
                {
                    procedure<T> proc = { vmin, vmax, vmax-vmin };
                    broker_transform::apply(target,device,proc);
                }
            }
        };

    }

}

#endif
