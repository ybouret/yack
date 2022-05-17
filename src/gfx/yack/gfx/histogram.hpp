
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

            histogram()                              throw(); //!< setup empty
            ~histogram()                             throw(); //!< cleanup
            histogram(const histogram & )            throw(); //!< copy
            histogram & operator=(const histogram &) throw(); //!< assign

            void reset()                  throw(); //!< zero all
            void merge(const histogram &) throw(); //!< add all

            size_t       & operator[](const uint8_t) throw();       //!< access
            const size_t & operator[](const uint8_t) const throw(); //!< access

            size_t cardinality() const throw(); //!< sum of all bins

            void    save(const string &filename) const; //!< save
            void    save(const char   *filename) const; //!< save wrapper
            uint8_t Otsu() const throw();               //!< Ostu threshold

        private:
            size_t bin[bins];

        };


    }

}

#endif
