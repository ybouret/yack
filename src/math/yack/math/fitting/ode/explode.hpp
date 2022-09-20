//! \file

#ifndef YACK_FITTING_EXPLODE_INCLUDED
#define YACK_FITTING_EXPLODE_INCLUDED 1

#include "yack/math/fitting/sequential.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! assembling ODE/Solver to make a sequential fit function
            //
            //__________________________________________________________________
            template <typename T> class explODE : public sequential<T,T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sequential<T,T> sequential_type; //!< alias to base class

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~explODE() throw(); //!< cleanup

            protected:
                explicit explODE(); //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(explODE);
            };


        }

    }
}


#endif

