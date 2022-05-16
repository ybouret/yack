
//! \file

#ifndef YACK_GFX_BROKER_NORMALIZE_INCLUDED
#define YACK_GFX_BROKER_NORMALIZE_INCLUDED 1

#include "yack/gfx/broker/transform.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration
        template <typename T> class pixrow; //!< forward declaration




        //______________________________________________________________________
        //
        //
        //! normalize
        //
        //______________________________________________________________________
        struct broker_normalize
        {
            template <typename T> struct procedure
            {
                T vmin;
                T vmax;
                T scal;
                void operator()(T &v) const throw();
            };

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
