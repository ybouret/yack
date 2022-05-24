//! \file

#ifndef YACK_GFX_EDGES_DETECTOR_INCLUDED
#define YACK_GFX_EDGES_DETECTOR_INCLUDED 1

#include "yack/gfx/pixmap.hpp"
#include "yack/gfx/blur.hpp"
#include "yack/gfx/broker.hpp"
#include "yack/gfx/filters.hpp"
#include "yack/gfx/blobs.hpp"

namespace yack
{

    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! edges algorithm
        //
        //______________________________________________________________________
        class edges : public pixmap<uint8_t>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef float       real_t; //!< internal computation
            typedef v2d<real_t> vertex; //!< internal vertices
            static const uint8_t strong = 0xff; //!< strong ridge
            static const uint8_t feeble = 0x7f; //!< feeble ridge

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit edges(const unit_t w, const unit_t h); //!< setup
            virtual ~edges() throw();                       //!< cleanup

            //__________________________________________________________________
            //
            // methods
            /**
             - apply deltas to compute gfield, vfield, gmax
             - keep only local gfield maxima, fill histogram with positive values
             - fill edges according to histogram thresholding
             - build blobs/labels and remove feeble only
             */
            //__________________________________________________________________
            void detect(broker              &device,
                        const filters       &deltas);

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            pixmap<float>  origin; //!< must be filled, becomes ridges
            pixmap<float>  gfield; //!< gradient amplitude
            pixmap<vertex> vfield; //!< gradient direction
            tagmap         labels; //!< edges label
            blobs          ridges; //!< dynamic ridges
            real_t         gmax;   //!< maximum gradient value
            

            


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(edges);
            uint8_t thinning(broker &);
            void    cleaving(broker &, const uint8_t up);
            void    tracking(broker &);
        };

    }

}

#endif

