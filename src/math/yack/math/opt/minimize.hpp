//! \file

#ifndef YACK_OPT_MINIMIZE_INCLUDED
#define YACK_OPT_MINIMIZE_INCLUDED 1

#include "yack/math/opt/bracket.hpp"
#include "yack/math/real-function.hpp"
#include "yack/math/timings.hpp"

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! 1D minimize
        //
        //______________________________________________________________________
        struct minimize
        {
            static bool verbose; //!< trigger verbosity
            
            //__________________________________________________________________
            //
            //! making one step
            //__________________________________________________________________
            template <typename T>
            struct move
            {
                
                //! constrained parabolic extrapolation
                static void run_(triplet<T>       &x,
                                 triplet<T>       &f,
                                 real_function<T> &F);

                //! wrapper to any callbale real function
                template <typename FUNC>
                static inline void run(triplet<T>       &x,
                                       triplet<T>       &f,
                                       FUNC             &F)
                {
                    typename real_function_of<T>::template call<FUNC> FF(F);
                    return run_(x,f,FF);
                }

            };

            //__________________________________________________________________
            //
            //! initialize and make step until convergence
            //__________________________________________________________________
            template <typename T>
            struct find
            {

                //! initialize and cycle up to XTOL, MTOL, FTOL
                static T run_(triplet<T>       &x,
                              triplet<T>       &f,
                              real_function<T> &F,
                              T                 xtol);

                //! wrapper to any callable real function
                template <typename FUNC>
                static inline T run(triplet<T>       &x,
                                    triplet<T>       &f,
                                    FUNC             &F,
                                    T                 xtol=0)
                {
                    typename real_function_of<T>::template call<FUNC> FF(F);
                    return run_(x,f,FF,xtol);
                }
            };


        };



    }

}

#endif
