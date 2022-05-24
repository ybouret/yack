//! \file

#ifndef YACK_GFX_EDGES_DETECTOR_INCLUDED
#define YACK_GFX_EDGES_DETECTOR_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/histogram.hpp"
#include "yack/gfx/blur.hpp"
#include "yack/gfx/broker.hpp"
#include "yack/gfx/filters.hpp"

namespace yack
{

    namespace graphic
    {
        
        class edges : public pixmap<uint8_t>
        {
        public:
            typedef float       real_t; //!< internal computation
            typedef v2d<real_t> vertex; //!< internal vertices
            static const uint8_t strong = 0xff;
            static const uint8_t feeble = 0x7f;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit edges(const unit_t w, const unit_t h);
            virtual ~edges() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void detect(broker              &device,
                        const filters       &deltas);

            //__________________________________________________________________
            //
            // membder
            //__________________________________________________________________
            pixmap<float>  origin; //!< must be filled, becomes ridges
            pixmap<float>  gfield; //!< gradient amplitude
            pixmap<vertex> vfield; //!< gradient direction
            real_t         gmax;   //!< maximum gradient value
            histogram      hist;   //!< histogram of valid gradient


            


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(edges);
            uint8_t thinning(broker &);
            void    cleaving(broker &, const uint8_t up);
        };

    }

}

#endif

