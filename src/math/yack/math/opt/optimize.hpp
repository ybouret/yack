
//! \file

#ifndef YACK_OPT_OPTIMIZE_INCLUDED
#define YACK_OPT_OPTIMIZE_INCLUDED 1

#include "yack/math/real-function.hpp"
#include "yack/math/triplet.hpp"

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! optimize in 1D
        //
        //______________________________________________________________________
        struct optimize
        {
            //__________________________________________________________________
            //
            //! trigger verbosity
            //__________________________________________________________________
            static bool verbose;

            //__________________________________________________________________
            //
            //! preprocessing
            //__________________________________________________________________
            enum preprocess
            {
                direct,
                inside,
                expand
            };

            //__________________________________________________________________
            //
            //! guess with x.a <= x.b <= x.c, f.b <= f.a, f.b <= f.c
            //__________________________________________________________________
            template <typename T> static
            T parabolic_guess(const triplet<T> &x, const triplet<T> &f) throw();


            //__________________________________________________________________
            //
            //! tighten a local minimum
            /**
             - iterate by controled parabolic guess and counter-reduction
             - requires TWO evaluations per call
             \param F a real function
             \param x x.a <= x.b <= x.c
             \param f f.b <= f.a, f.b <= f.c
             */
            //__________________________________________________________________
            template <typename T> static
            T tighten(real_function<T> &F,
                      triplet<T>       &x,
                      triplet<T>       &f,
                      bool             &decreased);


            //__________________________________________________________________
            //
            //! tighten wrapper
            //__________________________________________________________________
            template <typename T, typename FUNCTION> static inline
            T tighten_for(FUNCTION &F, triplet<T> &x, triplet<T> &f, bool &decreased)
            {
                typename real_function_of<T>::template call<FUNCTION> FF(F);
                return tighten<T>(FF,x,f,decreased);
            }

            //__________________________________________________________________
            //
            //! run
            /**
             - Tighten a local minimum up to convergence after
             (no) preprocessing.
             - The optimal value is always evaluated last.
             \param F a real function
             \param x x.a <= x.b <= x.c
             \param f f.b <= f.a, f.b <= f.c
             \param prolog what to do before optimization
             */
            //__________________________________________________________________
            template <typename T> static
            void run(real_function<T> &F, triplet<T> &x, triplet<T> &f, const preprocess prolog);

            //__________________________________________________________________
            //
            //! wrapper for run
            //__________________________________________________________________
            template <typename T, typename FUNCTION> static inline
            void run_for(FUNCTION &F, triplet <T> &x, triplet<T> &f, const preprocess prolog)
            {
                typename real_function_of<T>::template call<FUNCTION> FF(F);
                run(FF,x,f,prolog);
            }


        };

    }

}

#endif
