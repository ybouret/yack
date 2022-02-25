//! \file

#ifndef YACK_GFX_BITROWS_INCLUDED
#define YACK_GFX_BITROWS_INCLUDED 1

#include "yack/gfx/bitrow.hpp"
#include "yack/gfx/metrics.hpp"
#include "yack/gfx/article.hpp"


namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! rows for (bit|pix)map
        //
        //______________________________________________________________________
        class bitrows : public article
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit bitrows(const metrics &m, void *p);
            virtual ~bitrows() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const zero_flux zfw; //!< zero flux for width
            const zero_flux zfh; //!< zero flux for height
            const size_t    mem; //!< memory for rows
            bitrow         *row; //!< formatted rows
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bitrows);

        };
    }

}

#endif

