
//! \file

#ifndef YACK_GFX_BROKER_EXTREMA_INCLUDED
#define YACK_GFX_BROKER_EXTREMA_INCLUDED 1

#include "yack/gfx/broker.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration
        template <typename T> class pixrow; //!< forward declaration


        //______________________________________________________________________
        //
        //
        //! apply transformation
        //
        //______________________________________________________________________
        struct broker_extrema
        {
            //__________________________________________________________________
            //
            //! initialize memory, two words per context
            //__________________________________________________________________
            template <typename T> static inline
            void initialize(broker &device)
            {
                (*device).build<T>(2);
            }

            //__________________________________________________________________
            //
            //! find all local extrema for non-empty tiles
            //__________________________________________________________________
            template <typename T>
            static inline void look_up(const pixmap<T> &source, broker &device)
            {
                device(call<T>,(void*)&source);
            }

            //__________________________________________________________________
            //
            //! min/max reduction
            //__________________________________________________________________
            template <typename T> static inline
            void finalize(const broker &device)
            {
                const concurrent::loop &eng  = *device;
                size_t                  idx  = eng.size();
                const groove           &ini  = *eng[idx];
                T                       vmin = ini.get<T>(0);
                T                       vmax = ini.get<T>(1);
                while(--idx>0)
                {
                    const tiles  &part = device[idx];
                    if(part.size()<=0) break;

                    const groove &curr = *eng[idx];
                    {
                        const T tmp = curr.get<T>(0);
                        if(tmp<vmin) vmin = tmp;
                    }
                    {
                        const T tmp = curr.get<T>(1);
                        if(vmax<tmp) vmax = tmp;
                    }
                }
                std::cerr << "vmin=" << vmin << ", vmax=" << vmax << std::endl;
            }

        private:


            template <typename T> static inline
            void call(void          *args,
                      const tiles   &part,
                      const context &info,
                      lockable      &) throw()
            {
                if(part.size())
                {
                    const pixmap<T> &source = *static_cast< const pixmap<T> *>(args);
                    groove          &data   = *info;
                    T               &vmin   = data.get<T>(0);
                    T               &vmax   = data.get<T>(1);
                    const tile      *node   = part.head();
                    vmin = vmax = source(node->start);
                    for(;node;node=node->next)
                    {
                        coord            pos = node->start;
                        const pixrow<T> &src = source(pos.y);
                        for(size_t len=node->width;len>0;--len,++pos.x)
                        {
                            const T tmp = src(pos.x);
                            if(tmp<vmin) vmin=tmp;
                            if(vmax<tmp) vmax=tmp;
                        }
                    }
                }
            }

        };

    }

}

#endif

