//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fit/sample.hpp"
#include "yack/math/fit/least-squares/data.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {
            //__________________________________________________________________
            //
            //
            //! outcome of a fit round
            //
            //__________________________________________________________________
            enum round_result
            {
                round_success, //!< expanding search, decreased D2
                round_process, //!< shrinking search, decreased D2
                round_failure  //!< unable to decrease D2
            };

            //__________________________________________________________________
            //
            //
            //! least squares algorithm for a sample interface
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE>
            class least_squares : public least_squares_data<ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                typedef least_squares<ABSCISSA,ORDINATE>         self_type;       //!< alias
                typedef least_squares_data<ORDINATE>             base_type;       //!< alias
                typedef sample<ABSCISSA,ORDINATE>                sample_type;     //!< alias
                typedef sequential<ABSCISSA,ORDINATE>            sequential_type; //!< alias
                typedef functor<bool,TL1(writable<ORDINATE>&)>   control;         //!< true->ok, false->bad
                typedef real_function_of<ORDINATE>               real_func;       //!< alias

                //______________________________________________________________
                //
                // using
                //______________________________________________________________
                using base_type::verbose;
                using base_type::algo;
                using base_type::step;
                using base_type::atry;
                using base_type::curv;
                using base_type::shrinking;
                using base_type::lam;
                using base_type::clid;
                using base_type::D2;
                using base_type::drvs;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup

                inline virtual ~least_squares() throw() {}

                //! setup
                inline explicit least_squares() : base_type()
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

#include "yack/math/fit/least-squares/round.hxx"
#include "yack/math/fit/least-squares/fit.hxx"

                //______________________________________________________________
                //
                //! fit session wrapper
                //______________________________________________________________
                template <typename FUNC> inline
                bool fit_for(sample_type              &s,
                             FUNC                     &func,
                             writable<ORDINATE>       &aorg,
                             const readable<bool>     &used,
                             const readable<ORDINATE> &scal,
                             control                  *proc)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> F(func);
                    return fit(s,F,aorg,used,scal,proc);
                }

                //______________________________________________________________
                //
                //! per-parameter error computation
                //______________________________________________________________
                inline ORDINATE errors(writable<ORDINATE>       &aerr,
                                        sample_type              &s,
                                        sequential_type          &func,
                                        const readable<ORDINATE> &aorg,
                                        const readable<bool>     &used,
                                        const readable<ORDINATE> &scal)
                {

                    D2 = s.D2_full(func,aorg,used,drvs,scal);                // load D2 and compute metrics
                    this->compute_errors(aerr,s.curv,*s,used,s.dimension()); // deduce errors
                    return 0;
                }

                //______________________________________________________________
                //
                //! per-parameter error computation, wrapper
                //______________________________________________________________
                template <typename FUNC>
                inline ORDINATE errors_for(writable<ORDINATE>       &aerr,
                                           sample_type              &s,
                                           FUNC                     &func,
                                           const readable<ORDINATE> &aorg,
                                           const readable<bool>     &used,
                                           const readable<ORDINATE> &scal)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> F(func);
                    return errors(aerr,s,F,aorg,used,scal);
                }



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);
                
                struct call1D
                {
                    writable<ORDINATE>       &atry;
                    const readable<ORDINATE> &aorg;
                    const readable<ORDINATE> &step;
                    sample_type              &data;
                    sequential_type          &func;
                    
                    inline ORDINATE operator()(const ORDINATE u)
                    {
                        tao::v1::muladd(atry,aorg,u,step);
                        return data.D2(func,atry);
                    }
                };


            };

        }
    }

}

#endif

