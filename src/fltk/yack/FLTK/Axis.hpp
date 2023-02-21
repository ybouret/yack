#ifndef YACK_FLTK_AXIS_INCLUDED
#define YACK_FLTK_AXIS_INCLUDED 1

#include "yack/FLTK/Curve.hpp"

namespace yack
{
    namespace FLTK 
    {
        class Axis 
        {
        public:
            explicit Axis() noexcept; //!< 0-1
            virtual ~Axis() noexcept; 
            
            const double vmin;
            const double vmax;
            const double length;
            
            void set_min( double );
            void set_max( double );
            void set_range( double amin, double amax );
            
            void autoscaleY( const Curve &C, const double extra = 0.0 ); 
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Axis);
        };
        
    }
}

#endif
