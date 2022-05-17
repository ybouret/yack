
//! \file

#ifndef YACK_GFX_BROKER_SPLIT_INCLUDED
#define YACK_GFX_BROKER_SPLIT_INCLUDED 1

#include "yack/gfx/pixmaps.hpp"

namespace yack
{
    namespace graphic
    {

        //! splitting channels into stack
        struct broker_split
        {
            //! splitting algorithm
            template <typename PIXEL, typename T> static inline
            void build(pixmaps<T>          &target,
                       const pixmap<PIXEL> &source,
                       const size_t        *ch,
                       const size_t         nc,
                       broker              &device)
            {
                assert(yack_good(ch,nc));
                assert(nc<=target.size());
                struct task
                {
                    pixmaps<T>          &target;
                    const pixmap<PIXEL> &source;
                    const size_t        *ch;
                    const size_t         nc;
                    static inline void make(void         *args,
                                            const tiles   &part,
                                            const context &,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task                & self   = *static_cast<task *>(args);
                        pixmaps<T>          & target = self.target;
                        const pixmap<PIXEL> & source = self.source;
                        const size_t * const  ch     = self.ch;
                        const size_t          nc     = self.nc;
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord                pos = node->start;
                            const pixrow<PIXEL> &src = source(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                const T *s = (const T *) &src(pos.x);
                                for(size_t k=0;k<nc;)
                                {
                                    const size_t chan = ch[k]; // assert(chan<CHANNELS);
                                    target[++k](pos) = s[chan];
                                }
                            }
                        }
                    }
                };

                task todo = { target, source, ch, nc };
                device( task::make, &todo );
            }
        };
        
    }
    
}

#endif

