//! \file

#ifndef YACK_GFX_BROKER_FILTER_INCLUDED
#define YACK_GFX_BROKER_FILTER_INCLUDED 1

#include "yack/gfx/broker.hpp"
#include "yack/gfx/filter.hpp"
#include <cmath>

namespace yack
{
    namespace graphic
    {


        //______________________________________________________________________
        //
        //
        //! apply compiled filters
        //
        //______________________________________________________________________
        struct broker_filter
        {

            typedef filter::real_t real_t; //!< alias

            //__________________________________________________________________
            //
            //! apply target=F(source)
            //__________________________________________________________________
            template <
            typename TARGET,
            typename SOURCE> static inline
            void apply(pixmap<TARGET>       &target,
                       const pixmap<SOURCE> &source,
                       broker               &device,
                       const filter         &F)
            {
                struct task
                {
                    pixmap<TARGET>       &target;
                    const pixmap<SOURCE> &source;
                    const filter         &F;

                    static inline void make(void          *args,
                                            const tiles   &part,
                                            const context &,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task                 &self   = *static_cast<task *>(args);
                        pixmap<TARGET>       &target = self.target;
                        const pixmap<SOURCE> &source = self.source;
                        const filter         &F      = self.F;
                        
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord            pos = node->start;
                            pixrow<TARGET>   &tgt = target(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                tgt(pos.x) = static_cast<TARGET>(F(source,pos));
                            }
                        }
                    }
                };

                task todo = { target, source, F };
                device( task::make, &todo );
            }

            //__________________________________________________________________
            //
            //! apply target=sqrt(Fx(source)^2+Fy(source)^2) return global max
            //__________________________________________________________________
            template <
            typename TARGET,
            typename SOURCE> static inline
            real_t gradient(pixmap<TARGET>       &target,
                            const pixmap<SOURCE> &source,
                            broker               &device,
                            const filter         &Fx,
                            const filter         &Fy)
            {
                initialize(device);

                struct task
                {
                    pixmap<TARGET>       &target;
                    const pixmap<SOURCE> &source;
                    const filter         &Fx;
                    const filter         &Fy;

                    static inline void make(void          *args,
                                            const tiles   &part,
                                            const context &info,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task                 &self   = *static_cast<task *>(args);
                        pixmap<TARGET>       &target = self.target;
                        const pixmap<SOURCE> &source = self.source;
                        const filter         &Fx      = self.Fx;
                        const filter         &Fy      = self.Fy;
                        real_t                gmax    = 0;
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord            pos = node->start;
                            pixrow<TARGET>   &tgt = target(pos.y);
                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                const real_t gx = Fx(source,pos);
                                const real_t gy = Fy(source,pos);
                                const real_t g  = sqrt(gx*gx+gy*gy);
                                tgt(pos.x) = static_cast<TARGET>(g);
                                if(gmax<g) gmax = g;
                            }
                        }
                        (*info).as<real_t>() = gmax;
                    }
                };

                task todo = { target, source, Fx, Fy };
                device( task::make, &todo );

                return finalize(device);
            }

            //__________________________________________________________________
            //
            //! compute gradient amplitude and direction
            //__________________________________________________________________
            template <
            typename TARGET,
            typename SOURCE> static inline
            real_t gradient(pixmap<TARGET>         &target,
                            pixmap< v2d<TARGET> >  &gfield,
                            const pixmap<SOURCE>   &source,
                            broker                 &device,
                            const filter           &Fx,
                            const filter           &Fy)
            {
                initialize(device);

                struct task
                {
                    pixmap<TARGET>        &target;
                    pixmap< v2d<TARGET> > &gfield;
                    const pixmap<SOURCE>  &source;
                    const filter          &Fx;
                    const filter          &Fy;

                    static inline void make(void          *args,
                                            const tiles   &part,
                                            const context &info,
                                            lockable      &) throw()
                    {
                        assert(args);
                        task                  &self   = *static_cast<task *>(args);
                        pixmap<TARGET>        &target = self.target;
                        pixmap< v2d<TARGET> > &gfield = self.gfield;
                        const pixmap<SOURCE>  &source = self.source;
                        const filter          &Fx      = self.Fx;
                        const filter          &Fy      = self.Fy;
                        real_t                gmax    = 0;
                        for(const tile *node=part.head();node;node=node->next)
                        {
                            coord                   pos = node->start;
                            pixrow<TARGET>         &tgt = target(pos.y);
                            pixrow< v2d<TARGET> >  &vtx = gfield(pos.y);

                            for(size_t len=node->width;len>0;--len,++pos.x)
                            {
                                const real_t gx = Fx(source,pos);
                                const real_t gy = Fy(source,pos);
                                const real_t g2 = gx*gx+gy*gy;
                                if(g2>0)
                                {
                                    const real_t g  = sqrt(g2);
                                    tgt(pos.x) = static_cast<TARGET>(g);
                                    vtx(pos.x) = v2d<TARGET>(gx/g,gy/g);
                                    if(gmax<g) gmax = g;
                                }
                                else
                                {
                                    tgt(pos.x) = 0;
                                    vtx(pos.x) = v2d<TARGET>(0,0);
                                }
                            }
                        }
                        (*info).as<real_t>() = gmax;
                    }
                };

                task todo = { target, gfield, source, Fx, Fy };
                device( task::make, &todo );

                return finalize(device);
            }


        private:
            static void   initialize(broker &device);
            static real_t finalize(const broker &device) throw();
        };
    }

}

#endif

