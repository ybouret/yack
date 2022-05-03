//! \file

#ifndef YACK_GFX_BROKER_TRANSFORM_INCLUDED
#define YACK_GFX_BROKER_TRANSFORM_INCLUDED 1

#include "yack/gfx/broker.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration

        //______________________________________________________________________
        //
        //
        //! apply transformation
        //
        //______________________________________________________________________
        struct broker_transform
        {

            //! apply a 1 to 1 transformation
            template <typename T, typename U, typename PROC> static inline
            void apply(pixmap<T>       &target,
                       const pixmap<U> &source,
                       broker          &device,
                       PROC            &U_to_T) throw()
            {
                struct task {
                    pixmap<T>       &target;
                    const pixmap<U> &source;
                    PROC            &U_to_T;

                    static inline void make(void         *args,
                                            const tiles   &part,
                                            const context &,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task            &self   = *static_cast<task *>(args);
                        pixmap<T>       &target = self.target;
                        const pixmap<U> &source = self.source;
                        PROC            &U_to_T = self.U_to_T;
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord            pos = node->start;
                            pixrow<T>       &tgt = target(pos.y);
                            const pixrow<U> &src = source(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                U_to_T(tgt(pos.x),src(pos.x));
                            }
                        }
                    }
                };
                task todo = { target, source, U_to_T };
                device(task::make,&todo);
            }

        };
        

    }
}

#endif
