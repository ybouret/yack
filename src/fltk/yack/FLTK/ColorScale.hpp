#ifndef YACK_FLTK_COLOR_SCALE_INCLUDED
#define YACK_FLTK_COLOR_SCALE_INCLUDED 1

#include "yack/FLTK/Axis.hpp"
#include <FL/Fl_Box.H>

namespace yack
{
    namespace FLTK
    {
        
        class HColorScale : public Fl_Box
        {
        public:
            explicit HColorScale( int X, int Y, int W, int H, const char *l=0);
            virtual ~HColorScale() throw();
            
            Axis   xaxis;
            Axis   yaxis;
            Points data;
            Fl_Color color1;
            Fl_Color color2;
            
            virtual void draw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(HColorScale);
        };
        
    }
}

#endif
