
//! \file

#ifndef YACK_GFX_BROKER_FILTER_INCLUDED
#define YACK_GFX_BROKER_FILTER_INCLUDED 1

#include "yack/gfx/broker.hpp"
//#include "yack/gfx/pixel.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration
        template <typename T> class pixrow; //!< forward declaration
        //!
        struct broker_filter
        {

            template <
            typename TARGET,
            typename SOURCE,
            typename WORD> static inline
            void apply(pixmap<TARGET>       &target,
                       const pixmap<SOURCE> &source,
                       broker               &device )
            {
                struct task
                {
                    pixmap<TARGET>       &target;
                    const pixmap<SOURCE> &source;

                    static inline void make(void         *args,
                                            const tiles   &part,
                                            const context &,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task                 &self   = *static_cast<task *>(args);
                        pixmap<TARGET>       &target = self.target;
                        const pixmap<SOURCE> &source = self.source;
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord            pos = node->start;
                            //pixrow<T>       &tgt = target(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {

                            }
                        }
                    }
                };

                task todo = { target, source  };
                device( task::make, &todo );

            }

        };
    }

}

#endif

