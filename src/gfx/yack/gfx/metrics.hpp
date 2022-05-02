//! \file

#ifndef YACK_GFX_METRICS_INCLUDED
#define YACK_GFX_METRICS_INCLUDED 1

#include "yack/gfx/dimensions.hpp"


namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! metrics for a bitmap
        //
        //______________________________________________________________________
        class metrics : public dimensions
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            metrics(const unit_t width,
                    const unit_t height,
                    const unit_t depth) throw(); //!< W>0, H>0, D>0
            metrics(const metrics &) throw();    //!< copy
            virtual ~metrics() throw();          //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t n; //!< w*h
            const unit_t d; //!< depth
            const unit_t s; //!< stride = w * depth
            const size_t b; //!< minimal bytes  = stride * h = n * depth
        private:
            YACK_DISABLE_ASSIGN(metrics);
        };

    }

}

#endif
