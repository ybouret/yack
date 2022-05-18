
//! \file

#ifndef YACK_GFX_BROKER_BLUR_INCLUDED
#define YACK_GFX_BROKER_BLUR_INCLUDED 1

#include "yack/gfx/broker.hpp"
#include "yack/gfx/blur.hpp"

namespace yack
{
    namespace graphic
    {

        struct broker_blur
        {

            template <typename PIXEL, typename T, const size_t N> static inline
            void apply(pixmap<PIXEL>        &target,
                       const pixmap<PIXEL>  &source,
                       broker               &device,
                       const blurring       &B)
            {

                struct task
                {
                    pixmap<PIXEL>       &target;
                    const pixmap<PIXEL> &source;
                    const blurring      &B;

                    static inline void make(void          *args,
                                            const tiles   &part,
                                            const context &,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task                 &self   = *static_cast<task *>(args);
                        pixmap<PIXEL>        &target = self.target;
                        const pixmap<PIXEL>  &source = self.source;
                        const blurring       &B      = self.B;
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord                pos = node->start;
                            pixrow<PIXEL>       &tgt = target(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                B.apply<PIXEL,T,N>(tgt(pos.x),source,pos);
                            }
                        }
                        
                    }
                };

                task todo = { target, source, B };
                device(task::make,&todo);
            }

            template <typename T> static
            void fuzz(pixmap<T> &, const pixmap<T> &, broker &, const blurring &);

        };

    }

}

#endif
