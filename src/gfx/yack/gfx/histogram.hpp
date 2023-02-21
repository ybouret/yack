
//! \file

#ifndef YACK_GFX_HISTOGRAM_INCLUDED
#define YACK_GFX_HISTOGRAM_INCLUDED 1

#include "yack/gfx/rgb.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! making histogram from pixels
        //
        //______________________________________________________________________
        class histogram
        {
        public:
            static const size_t bins = 256; //!< on a [0:255] level set

            histogram()                              noexcept; //!< setup empty
            ~histogram()                             noexcept; //!< cleanup
            histogram(const histogram & )            noexcept; //!< copy
            histogram & operator=(const histogram &) noexcept; //!< assign

            void reset()                  noexcept; //!< zero all
            void merge(const histogram &) noexcept; //!< add all

            size_t       & operator[](const uint8_t) noexcept;       //!< access
            const size_t & operator[](const uint8_t) const noexcept; //!< access

            size_t cardinality() const noexcept; //!< sum of all bins

            void    save(const string &filename) const; //!< save
            void    save(const char   *filename) const; //!< save wrapper
            uint8_t Otsu() const noexcept;               //!< Ostu threshold

        private:
            size_t bin[bins];

        };


    }

}

#endif
