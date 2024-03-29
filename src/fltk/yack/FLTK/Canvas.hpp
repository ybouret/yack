#ifndef YACK_FLTK_CANVAS_INCLUDED
#define YACK_FLTK_CANVAS_INCLUDED 1

#include "yack/FLTK/Axis.hpp"
#include <FL/Fl_Box.H>

namespace yack
{
    namespace FLTK
    {
        
        class Canvas : public Fl_Box
        {
        public:
            explicit Canvas( int X, int Y, int W, int H, const char *l = 0 );
            virtual ~Canvas() noexcept;
            
            Axis   xaxis;
            Axis   yaxis;
            Axis   y2axis;
            Curves curves;
            Curves curves2;
            
            virtual void draw();
            
        private:
            static 
            void __draw( const Fl_Box &box, const Axis &X, const Axis &Y, const Curves &C );
            YACK_DISABLE_COPY_AND_ASSIGN(Canvas);
        };
        
    }
    
}

#endif
