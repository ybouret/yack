
//! \file

#ifndef YACK_COLOR_RGBA_TO_DATA_INCLUDED
#define YACK_COLOR_RGBA_TO_DATA_INCLUDED 1

#include "yack/color/rgba/conv.hpp"

namespace yack
{
    namespace color
    {

        //______________________________________________________________________
        //
        //
        //! base class to convert RGBA to some data
        //
        //______________________________________________________________________
        class rgba_to_data : public rgba_conv
        {
        public:
            //! put data at address from color
            virtual void operator()(void *addr, const rgba &) const throw() = 0;

            //! cleanup
            virtual ~rgba_to_data() throw();
            
        protected:
            //! setup
            explicit rgba_to_data(const unit_t depth) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rgba_to_data);
        };

    }

}

#endif

