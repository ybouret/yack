
//! \file

#ifndef YACK_GFX_BROKER_NORMALIZE_INCLUDED
#define YACK_GFX_BROKER_NORMALIZE_INCLUDED 1

#include "yack/gfx/broker/transform.hpp"
#include "yack/gfx/broker/extrema.hpp"

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
                void operator()(T &v) const noexcept; //!< apply
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

            //! normalize with 0-vmax
            template <typename T> static inline
            void apply(pixmap<T> &target, broker &device, const T vmax)
            {
                apply(target,device,T(0),vmax);
            }

            //! auto normalize
            template <typename T> static inline
            void apply(pixmap<T> &target, broker &device)
            {
                const T * const v = broker_extrema::search(target,device);
                apply(target,device,v[0],v[1]);
            }



        };

    }

}

#endif
