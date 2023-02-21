#include "yack/gfx/edges/detector.hpp"
#include "yack/gfx/broker/histogram.hpp"
#include <cmath>

namespace yack
{
    namespace graphic
    {

        uint8_t edges:: thinning(broker &device)
        {
            histogram hist;
            broker_histogram::initialize(hist,device);

            //------------------------------------------------------------------
            struct task
            {
                pixmap<float>        &target;
                const pixmap<float>  &gfield;
                const pixmap<vertex> &vfield;
                float                 gmax;

                static inline void make(void          *args,
                                        const tiles   &part,
                                        const context &info,
                                        lockable      &) noexcept
                {
                    assert(args);
                    task                 &self   = *static_cast<task *>(args);
                    pixmap<float>        &target = self.target;
                    const pixmap<float>  &gfield = self.gfield;
                    const pixmap<vertex> &vfield = self.vfield;
                    const float           gmax   = self.gmax;
                    histogram            &hist   = info->as<histogram>();

                    for(const tile *node=part.head();node;node=node->next)
                    {
                        coord                  pos = node->start;
                        pixrow<float>         &tgt = target(pos.y);
                        const pixrow<float>   &grd = gfield(pos.y);
                        const pixrow<vertex>  &vtx = vfield(pos.y);
                        for(size_t len=node->width;len>0;--len,++pos.x)
                        {
                            const vertex v  = vtx(pos.x);
                            const unit_t dx = static_cast<unit_t>(floor(v.x+0.5f));
                            const unit_t dy = static_cast<unit_t>(floor(v.y+0.5f));
                            if( absolute(dx) + absolute(dy) <= 0)
                            {
                                tgt(pos.x)=0;
                            }
                            else
                            {
                                const float  g0 = grd(pos.x);
                                const float  gm = gfield[ coord(pos.x-dx,pos.y-dy) ];
                                const float  gp = gfield[ coord(pos.x+dx,pos.y+dy) ];
                                if(g0>=gm&&g0>=gp)
                                {
                                    const float f = g0/gmax;
                                    tgt(pos.x) = f;
                                    ++hist[ uint8_t( floorf(255.0f*f+0.5f)) ];
                                }
                                else
                                {
                                    tgt(pos.x) = 0;
                                }
                            }
                        }

                    }

                }
            };
            //------------------------------------------------------------------
            
            task todo = { origin, gfield, vfield, gmax };
            device(task::make,&todo);

            broker_histogram::finalize(hist,device);

            return hist.Otsu();
        }
    }

}

