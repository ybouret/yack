//! \file

#ifndef YACK_FIT_EXPLODE_INCLUDED
#define YACK_FIT_EXPLODE_INCLUDED 1

#include "yack/math/fit/sequential.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
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

