


//! \file

#ifndef YACK_COLOR_FROM_BYTE_INCLUDED
#define YACK_COLOR_FROM_BYTE_INCLUDED 1

#include "yack/color/data-to-rgba.hpp"

namespace yack
{
    namespace color
    {
        
        //______________________________________________________________________
        //
        //
        //! byte->rgba
        //
        //______________________________________________________________________
        class from_byte : public data_to_rgba
        {
        public:
            explicit from_byte() throw(); //!< depth=sizeof(rgba)
            virtual ~from_byte() throw(); //!< cleanup
            
            //!  byte  -> (r,g,b,255)
            virtual rgba operator()(const void *addr) const throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(from_byte);
        };
        
    }
    
}

#endif

