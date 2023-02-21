#include "yack/gfx/edges/detector.hpp"
#include "yack/gfx/broker/filter.hpp"
#include "yack/gfx/histogram.hpp"

namespace yack
{

    namespace graphic
    {


        edges:: ~edges() noexcept
        {
        }

        edges:: edges(const unit_t w, const unit_t h) :
        pixmap<uint8_t>(w,h),
        origin(w,h),
        gfield(w,h),
        vfield(w,h),
        labels(w,h),
        ridges(),
        gmax(0)
        {
        }


        void edges:: detect(broker              &device,
                            const filters       &deltas)
        {
            device->ensure(1,sizeof(histogram));
            gmax = broker_filter::gradient(gfield,vfield,origin,device,*deltas.X,*deltas.Y);
            const uint8_t up = thinning(device);
            cleaving(device,up);
            tracking(device);
        }

        static inline
        bool is_strong_ridge(const cnode *head, const pixmap<uint8_t> &edges) noexcept
        {
            while(head)
            {
                if( edges(**head) >= edges::strong) return true;
                head = head->next;
            }
            return false;
        }


        void edges:: tracking(broker &device)
        {
            ridges.build(labels, device, *this, 8);
            blobs_ keep;
            while(ridges.size)
            {
                blob *b = ridges.pop_front();
                if(is_strong_ridge(b->head,*this))
                {
                    keep.push_back(b);
                }
                else
                {
                    for(const cnode *node=b->head;node;node=node->next)
                    {
                        const coord pos = **node;
                        (*this)(pos) = 0;
                        labels(pos)  = 0;
                    }
                    delete b;
                }
            }

            ridges.swap_with(keep);
            ridges.relabel(labels);

        }

    }

}
