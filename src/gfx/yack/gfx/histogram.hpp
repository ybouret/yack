
//! \file

#ifndef YACK_GFX_BROKER_INCLUDED
#define YACK_GFX_BROKER_INCLUDED 1

#include "yack/gfx/rgb.hpp"

namespace yack
{
    namespace graphic
    {

        class histogram
        {
        public:
            static const size_t bins = 256;

            histogram()  throw();
            ~histogram() throw();
            histogram(const histogram & ) throw();
            histogram & operator=(const histogram &) throw();

            void reset()                  throw();
            void merge(const histogram &) throw();

            size_t       & operator[](const uint8_t) throw();
            const size_t & operator[](const uint8_t) const throw();

        private:
            size_t bin[bins];

        };


    }

}

#endif
