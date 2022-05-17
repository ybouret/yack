
//! \file

#ifndef YACK_GFX_BROKER_HISTOGRAM_INCLUDED
#define YACK_GFX_BROKER_HISTOGRAM_INCLUDED 1

#include "yack/gfx/broker.hpp"
#include "yack/gfx/histogram.hpp"
#include "yack/gfx/pixel.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration
        template <typename T> class pixrow; //!< forward declaration

        template <typename T> struct selector
        {
            typedef void (*callback)(T &, const T&);
            static inline void keep(T &tgt, const T &src)
            {
                tgt = src;
            }

            static inline void drop(T &tgt, const T &)
            {
                tgt = pixel<T>::zero();
            }
        };

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




            template <typename T, typename PROC>  static inline
            void process(pixmap<T>                     &target,
                         const pixmap<T>               &source,
                         const uint8_t                  thresh,
                         broker                        &device,
                         PROC                          &toByte,
                         typename selector<T>::callback foreground,
                         typename selector<T>::callback background)
            {
                typedef typename selector<T>::callback callback;
                assert(NULL!=foreground);
                assert(NULL!=background);
                struct task
                {
                    pixmap<T>       &target;
                    const pixmap<T> &source;
                    uint8_t          thresh;
                    PROC            &toByte;
                    callback         foreground;
                    callback         background;
                    static inline void make(void          *args,
                                            const tiles   &part,
                                            const context &,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task            &self       = *static_cast<task *>(args);
                        pixmap<T>       &target     = self.target;
                        const pixmap<T> &source     = self.source;
                        PROC            &toByte     = self.toByte;
                        callback         foreground = self.foreground;
                        callback         background = self.background;
                        const uint8_t    threshold  = self.thresh;

                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord            pos = node->start;
                            pixrow<T>       &tgt = target(pos.y);
                            const pixrow<T> &src = source(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                const T      &value = src(pos.x);
                                const uint8_t level = toByte(value);
                                if(level>=threshold)
                                {
                                    foreground(tgt(pos.x),value);
                                }
                                else
                                {
                                    background(tgt(pos.x),value);
                                }
                            }

                        }
                    }
                };

                task todo = { target, source, thresh, toByte, foreground, background };
                device(task::make,&todo);
            }

        };

    }

}

#endif
