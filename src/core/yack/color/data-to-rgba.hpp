//! \file

#ifndef YACK_COLOR_DATA_TO_RGBA_INCLUDED
#define YACK_COLOR_DATA_TO_RGBA_INCLUDED 1

#include "yack/color/rgba/conv.hpp"

namespace yack
{
    namespace color
    {
        //______________________________________________________________________
        //
        //
        //! interface to convert data to rgba
        //
        //______________________________________________________________________
        class data_to_rgba : public rgba_conv
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________

            //! get rgba from data at address 
            virtual rgba operator()(const void *addr) const throw() = 0;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~data_to_rgba() throw();
        protected:
            //! setup
            explicit data_to_rgba(const unit_t depth) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_to_rgba);
        };

    }

}

#endif

