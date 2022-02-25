//! \file

#ifndef YACK_GFX_BITROWS_INCLUDED
#define YACK_GFX_BITROWS_INCLUDED 1

#include "yack/gfx/bitrow.hpp"
#include "yack/gfx/metrics.hpp"
#include "yack/object.hpp"
#include "yack/counted.hpp"

namespace yack
{
    namespace graphic
    {

        class bitrows : public object, public counted
        {
        public:
            explicit bitrows(const metrics &m, void *p);
            virtual ~bitrows() throw();

            const zero_flux zfw;
            const zero_flux zfh;

            bitrow       & operator()(const unit_t y)       throw();
            const bitrow & operator()(const unit_t y) const throw();
            bitrow       & operator[](const unit_t y)       throw();
            const bitrow & operator[](const unit_t y) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bitrows);
            size_t          mem;
            bitrow         *row;
        };
    }

}

#endif

