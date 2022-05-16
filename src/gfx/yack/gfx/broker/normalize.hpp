
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
            template <typename T> static inline
            void apply(pixmap<T> &target, const broker &device, const T vmax)
            {
                struct task
                {
                    T vmax;
                    inline void operator()(T &) const throw()
                    {
                    }
                };

                if(vmax>0)
                {
                    task proc = { vmax };
                    broker_transform::apply(target,device,proc);
                }
            }
        };

    }

}

#endif
