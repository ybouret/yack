//! \file

#ifndef YACK_BROKER3x3_INCLUDED
#define YACK_BROKER3x3_INCLUDED 1

#include "yack/gfx/broker.hpp"
#include "yack/gfx/coord.hpp"

namespace yack
{
    namespace graphic
    {

        template <typename T> class pixmap; //!< forward declaration
        template <typename T> class pixrow; //!< forward declaration

        struct broker3x3
        {



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
