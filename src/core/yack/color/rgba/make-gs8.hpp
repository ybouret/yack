


//! \file

#ifndef YACK_COLOR_MAKE_GS8_INCLUDED
#define YACK_COLOR_MAKE_GS8_INCLUDED 1

#include "yack/color/rgba-to-data.hpp"

namespace yack
{
    namespace color
    {

        //______________________________________________________________________
        //
        //
        //! rgba -> float
        //
        //______________________________________________________________________
        class make_gs8 : public rgba_to_data
        {
        public:
            //! table of bytes
            static const uint8_t table[3*255+1];

            explicit make_gs8() throw(); //!< depth=sizeof(uint8_t)
            virtual ~make_gs8() throw(); //!< cleanup

            //! (r,g,b) -> (uint8_t)
            virtual void operator()(void *addr, const rgba &) const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(make_gs8);
        };

    }

}

#endif

