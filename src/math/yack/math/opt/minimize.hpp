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
            //
            //! find parabolic min for x in increasing order, f.b<=f.a, f.b<=f.c
            //
            //__________________________________________________________________
            template <typename T>
            static T parabolic_guess(const triplet<T> &x,
                                     const triplet<T> &f) throw();



            //__________________________________________________________________
            //
            //
            //! making one step
            //
            //__________________________________________________________________
            template <typename T>
            struct move
            {

                //______________________________________________________________
                //
                //! constrained parabolic interpolation
                //______________________________________________________________
                static void run(real_function<T> &F,
                                triplet<T>       &x,
                                triplet<T>       &f);

                //______________________________________________________________
                //
                //! wrapper to any callable real function
                //______________________________________________________________
                template <typename FUNC>
                static inline void run_for(FUNC             &F,
                                           triplet<T>       &x,
                                           triplet<T>       &f)
                {
                    typename real_function_of<T>::template call<FUNC> FF(F);
                    return run(FF,x,f);
                }

            };

            //__________________________________________________________________
            //
            //
            //! initialize and make step until convergence
            //
            //__________________________________________________________________
            template <typename T>
            struct find
            {

                //______________________________________________________________
                //
                //! initialize and cycle up to XTOL, MTOL, FTOL
                /**
                 \return x.b such that f.b = F(x.b) is the last evaluated value
                 */
                //______________________________________________________________
                static T run(real_function<T> &F,
                             triplet<T>       &x,
                             triplet<T>       &f,
                             T                 xtol);

                //______________________________________________________________
                //
                //! wrapper to any callable real function
                //______________________________________________________________
                template <typename FUNC>
                static inline T run_for(FUNC             &F,
                                        triplet<T>       &x,
                                        triplet<T>       &f,
                                        T                 xtol=0)
                {
                    typename real_function_of<T>::template call<FUNC> FF(F);
                    return run(FF,x,f,xtol);
                }
            };


        };



    }

}

#endif
