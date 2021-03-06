//! \file

#ifndef YACK_BROKER3x3_INCLUDED
#define YACK_BROKER3x3_INCLUDED 1

#include "yack/gfx/broker.hpp"
#include "yack/gfx/coord.hpp"
#include "yack/gfx/pixel.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration
        template <typename T> class pixrow; //!< forward declaration

        //__________________________________________________________________
        //
        //
        //! apply 3x3 filters
        //
        //__________________________________________________________________
        struct broker3x3
        {

            //__________________________________________________________________
            //
            //! call proc after collecting 9 pixels
            //__________________________________________________________________
            template <typename T, typename U, typename F3X3> static inline
            void apply(pixmap<T>       &target,
                       const pixmap<U> &source,
                       broker          &device,
                       F3X3            &fcn3x3)
            {
                initialize<U>(device);

                //--------------------------------------------------------------
                struct task {
                    pixmap<T>       &target;
                    const pixmap<U> &source;
                    F3X3            &fcn3x3;

                    static inline void make(void         *args,
                                            const tiles   &part,
                                            const context &info,
                                            lockable      &) throw()
                    {
                        assert(args);
                        assert(info.owns_local_memory());
                        assert(info.local_memory_size()>=9*sizeof(U));
                        
                        task            &self   = *static_cast<task *>(args);
                        pixmap<T>       &target = self.target;
                        const pixmap<U> &source = self.source;
                        F3X3            &fcn3x3 = self.fcn3x3;
                        U               *pixels = & (info->as<U>());
                        
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord            pos = node->start;
                            pixrow<T>       &tgt = target(pos.y);
                            const pixrow<U> &src = source(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                for(size_t k=0;k<8;++k)
                                {
                                    pixels[k] = source[pos+coords::delta[k]];
                                }
                                pixels[8] = src(pos.x);
                                fcn3x3(tgt(pos.x),pixels);
                            }
                        }
                    }
                };
                //--------------------------------------------------------------

                task todo = { target, source, fcn3x3 };
                device(task::make,&todo);
            }

            //! workspace=max(source), target=min(workspace)
            template <typename T> static inline
            void close(pixmap<T> &target, const pixmap<T> &source, broker &device, pixmap<T> &workspace)
            {
                apply(workspace,source,device,pixel<T>::maximum3x3);
                apply(target,workspace,device,pixel<T>::minimum3x3);
            }

            //! workspace=min(source), target=max(workspace)
            template <typename T> static inline
            void erode(pixmap<T> &target, const pixmap<T> &source, broker &device, pixmap<T> &workspace)
            {
                apply(workspace,source,device,pixel<T>::minimum3x3);
                apply(target,workspace,device,pixel<T>::maximum3x3);
            }



        private:
            //! initialize local memory
            template <typename U> static inline
            void initialize(broker &device)
            {
                (*device).build<U>(3*3);
            }
        };
    }
}

#endif
