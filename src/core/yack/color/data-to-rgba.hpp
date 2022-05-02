//! \file

#ifndef YACK_COLOR_DATA_TO_RGBA_INCLUDED
#define YACK_COLOR_DATA_TO_RGBA_INCLUDED 1

#include "yack/color/rgba-conv.hpp"

namespace yack
{
    namespace color
    {

        class data_to_rgba : public rgba_conv
        {
        public:
            //
            

            // C++

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

