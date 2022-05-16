
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

        //! v/vmax
        template <typename T> void normalize_pixel(T &, const T) throw();

        

        //______________________________________________________________________
        //
        //
        //! normalize
        //
        //______________________________________________________________________
        struct broker_normalize
        {
            template <typename T> struct procedure1
            {
                T vmax;
                inline void operator()(T &v) const throw() {
                    normalize_pixel(v,vmax);
                }
            };

            template <typename T> static inline
            void apply(pixmap<T> &target, broker &device, const T vmax)
            {
                if(vmax>0)
                {
                    procedure1<T> proc = { vmax };
                    broker_transform::apply(target,device,proc);
                }
            }
        };

    }

}

#endif
