//! \file

#ifndef YACK_MATH_JACOBIAN_INCLUDED
#define YACK_MATH_JACOBIAN_INCLUDED 1

#include "yack/math/fcn/derivative.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace math
    {
        
        //______________________________________________________________________
        //
        //
        //! numerical gradient from derivative
        //
        //______________________________________________________________________
        template <typename T>
        class jacobian
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename derivative<T>::pointer derivative_ptr; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            virtual ~jacobian() throw();
            
            //! setup
            explicit jacobian(const derivative_ptr &, const T = 1e-4) throw();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! generic computation
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            derivative_ptr drvs; //!< 1D numerical derivative
            T              scal; //!< initial scaling
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(jacobian);
            
        };
        
    }
    
}

#endif
