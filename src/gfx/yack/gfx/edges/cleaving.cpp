#include "yack/gfx/edges/detector.hpp"
#include <cmath>

namespace yack
{
    namespace graphic
    {
        void edges:: cleaving(broker &device, const uint8_t up)
        {

            struct task
            {
                pixmap<uint8_t>     &target;
                const pixmap<float> &ridges;
                uint8_t              up;
                uint8_t              lo;

                static inline void make(void          *args,
                                        const tiles   &part,
                                        const context & ,
                                        lockable      &) noexcept
                {
                    assert(args);
                    task                 &self   = *static_cast<task *>(args);
                    pixmap<uint8_t>      &target = self.target;
                    const pixmap<float>  &ridges = self.ridges;
                    const uint8_t         lo     = self.lo;
                    const uint8_t         up     = self.up;
                    for(const tile *node=part.head();node;node=node->next)
                    {
                        coord                  pos = node->start;
                        pixrow<uint8_t>       &tgt = target(pos.y);
                        const pixrow<float>   &src = ridges(pos.y);
                        for(size_t len=node->width;len>0;--len,++pos.x)
                        {
                            const float   f = src(pos.x);
                            const uint8_t u = static_cast<uint8_t>( floorf(255.0f * f +0.5f) );
                            if(u>=up) {
                                tgt(pos.x) = strong;
                                continue;
                            }
                            if(u>=lo) {
                                tgt(pos.x) = feeble;
                                continue;
                            }
                            tgt(pos.x) = 0;
                        }
                    }

                }

            };

            task todo = { *this, origin, up, static_cast<uint8_t>(up>>2) };
            device(task::make,&todo);

        }

    }

}


