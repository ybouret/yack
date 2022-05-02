

//! \file

#ifndef YACK_COLOR_MAKE_GSF_INCLUDED
#define YACK_COLOR_MAKE_GSF_INCLUDED 1

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
        class make_gsf : public rgba_to_data
        {
        public:
            static const float table[3*255+1];
            
            explicit make_gsf() throw(); //!< depth=sizeof(float)
            virtual ~make_gsf() throw(); //!< cleanup

            //! (r,g,b) -> (float)
            virtual void operator()(void *addr, const rgba &) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(make_gsf);
        };

    }

}

#endif

