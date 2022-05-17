

//! \file

#ifndef YACK_COLOR_FROM_FLOAT_INCLUDED
#define YACK_COLOR_FROM_FLOAT_INCLUDED 1

#include "yack/color/data-to-rgba.hpp"

namespace yack
{
    namespace color
    {
        
        //______________________________________________________________________
        //
        //
        //! float->rgba
        //
        //______________________________________________________________________
        class from_float : public data_to_rgba
        {
        public:
            explicit from_float() throw(); //!< depth=sizeof(float)
            virtual ~from_float() throw(); //!< cleanup
            
            //!  float  -> (r,g,b,255)
            virtual rgba operator()(const void *addr) const throw();

            //! for transform
            inline void operator()(rgba &c, const float f) const throw()
            {
                c = (*this)( &f );
            }
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(from_float);
        };
        
    }
    
}

#endif
