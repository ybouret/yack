
//! \file

#ifndef YACK_GFX_BROKER_HISTOGRAM_INCLUDED
#define YACK_GFX_BROKER_HISTOGRAM_INCLUDED 1

#include "yack/gfx/broker.hpp"
#include "yack/gfx/histogram.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration
        template <typename T> class pixrow; //!< forward declaration

        //______________________________________________________________________
        //
        //
        //! operation for histogram
        //
        //______________________________________________________________________
        struct broker_histogram
        {
            //! hist.reset(), allocate memory for device
            static void initialize(histogram &hist, const broker &device);

            //! merget local histogram into hist
            static void finalize(histogram &hist, const broker &device);

            //! initialize/accumulate/finalize
            template <typename T, typename PROC> static inline
            void compute(histogram       &hist,
                         const pixmap<T> &source,
                         broker          &device,
                         PROC            &toByte)
            {
                initialize(hist,device);

                struct task
                {
                    const pixmap<T> &source;
                    PROC            &toByte;

                    static inline void make(void          *args,
                                            const tiles   &part,
                                            const context &info,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task            &self   = *static_cast<task *>(args);
                        const pixmap<T> &source = self.source;
                        PROC            &toByte = self.toByte;
                        histogram       &h      = (*info).as<histogram>();

                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord            pos = node->start;
                            const pixrow<T> &src = source(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                ++h[ toByte(src(pos.x)) ];
                            }
                        }
                    }
                };
                task todo = { source, toByte };
                device(task::make,&todo);

                finalize(hist,device);
            }
            
        };

    }

}

#endif
