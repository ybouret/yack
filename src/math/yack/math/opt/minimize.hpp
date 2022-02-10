//! \file

#ifndef YACK_OPT_MINIMIZE_INCLUDED
#define YACK_OPT_MINIMIZE_INCLUDED 1

#include "yack/math/opt/bracket.hpp"
#include "yack/math/real-function.hpp"

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
            //__________________________________________________________________
            //
            //! trigger verbosity
            //__________________________________________________________________
            static bool verbose;

            //__________________________________________________________________
            //
            //! prolog to run
            //__________________________________________________________________
            enum prolog
            {
                direct, //!< no prolog
                inside, //!< bracket inside x.a,x.c with f.a,f.c
                expand  //!< bracket expand from x.a,x.b,x.c with f.a,f.b,f.c computed
            };

            //__________________________________________________________________
            //
            //
            //! find parabolic min for x in increasing order, f.b<=f.a, f.b<=f.c
            //
            //__________________________________________________________________
            template <typename T>
            static T parabolic_guess(const triplet<T> &x, const triplet<T> &f) throw();


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
                 - direct: x.a <= x.b <= x.c and f.b <= f.a and f.b <= f.c
                 - inside: x.a,f.a and x.c,f.c are precomputed
                 - expand: all triplets are precomputed
                 \return x.b such that f.b = F(x.b) is the last evaluated value
                 */
                //______________________________________________________________
                static T run(real_function<T> &F,
                             triplet<T>       &x,
                             triplet<T>       &f,
                             const prolog      flag,
                             T                 xtol);

                //______________________________________________________________
                //
                //! wrapper to any callable real function
                //______________________________________________________________
                template <typename FUNC>
                static inline T run_for(FUNC             &F,
                                        triplet<T>       &x,
                                        triplet<T>       &f,
                                        const prolog      flag,
                                        T                 xtol=0)
                {
                    typename real_function_of<T>::template call<FUNC> FF(F);
                    return run(FF,x,f,flag,xtol);
                }
            };


        };



    }

}

#endif
