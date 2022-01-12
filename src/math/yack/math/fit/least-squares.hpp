//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fit/sample.hpp"
#include "yack/math/fit/least-squares-data.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {

            //! identify cycle result
            enum cycle_result
            {
                cycle_success, //!< expanding search, decreased D2
                cycle_process, //!< shrinking search, decreased D2
                cycle_failure  //!< unable to decrease D2
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
                //! perform one cycle
                /**
                 \param s a sample
                 \param p current power indicator
                 \param func a sequential fit function
                 \param aorg current parameters
                 \param used currently used parameters
                 \param scal scaling factor for gradient of func
                 \param ctrl control function
                 */
                //______________________________________________________________
                cycle_result cycle(sample_type              &s,
                                   unit_t                   &p,
                                   sequential_type          &func,
                                   writable<ORDINATE>       &aorg,
                                   const readable<bool>     &used,
                                   const readable<ORDINATE> &scal,
                                   control                  *ctrl)
                {
                    assert(aorg.size()==used.size());
                    assert(aorg.size()==scal.size());

                    //----------------------------------------------------------
                    //
                    // initialize memory, set shrinking to false
                    // and check power
                    //
                    //----------------------------------------------------------
                    const size_t     npar = aorg.size();
                    const variables &vars = *s;
                    this->initialize(npar);
                    p = clamp(this->pmin,p,this->pmax);

                    //----------------------------------------------------------
                    //
                    // prepare a real function
                    //
                    //----------------------------------------------------------
                    call1D  f1d = { atry, aorg, step, s, func };
                    typename real_func:: template call<call1D> g(f1d);

                    //----------------------------------------------------------
                    //
                    // compute all metrics @aorg: COSTLY
                    //
                    //----------------------------------------------------------
                    const ORDINATE D2ini = D2 = s.D2_full(func,aorg,used,drvs,scal);
                    s.finalize();


                    YACK_LSF_PRINTLN(clid << " cycle [p=" << p << "] => lambda=" << lam[p]);
                    if(verbose) vars.display(std::cerr,aorg,"\taorg.");
                    YACK_LSF_PRINTLN("D2ini = " << D2ini);


                    //----------------------------------------------------------
                    //
                    // making the best out of full metrics
                    //
                    //----------------------------------------------------------

                CURV:
                    //----------------------------------------------------------
                    //
                    // try compute curvature
                    //
                    //----------------------------------------------------------
                    if(!this->compute_curvature(p,s.curv,vars,used))
                    {
                        //------------------------------------------------------
                        // singular
                        //------------------------------------------------------
                        YACK_LSF_PRINTLN(clid << " singular parameters");
                        return cycle_failure;
                    }

                    //----------------------------------------------------------
                    //
                    // from estimated curvature and gradient, compute step
                    //
                    //----------------------------------------------------------
                    tao::v1::load(step,s.beta);
                    algo->solve(curv,step);

                    YACK_LSF_PRINTLN(clid << " step [p=" << p << "]");
                    if(verbose) vars.display(std::cerr,step,"\tstep.");

                    //----------------------------------------------------------
                    //
                    // compute trial parameters
                    //
                    //----------------------------------------------------------
                    tao::v1::add(atry,aorg,step);             // atry=aorg+step

                    //----------------------------------------------------------
                    //
                    // check post-processing
                    //
                    //----------------------------------------------------------
                    if(ctrl)
                    {

                        if( !(*ctrl)(atry) )
                        {
                            // something happened
                            YACK_LSF_PRINTLN(clid << " post-processing warning");
                            if(!this->shrink(p))
                            {
                                YACK_LSF_PRINTLN(clid << " post-processing caused failure");
                                return cycle_failure;
                            }
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // recompute step and find D2try
                    //
                    //----------------------------------------------------------
                    tao::v1::sub(step,atry,aorg);       // recompute step in any case
                    ORDINATE D2try = s.D2(func,atry);   // compute trial value

                    YACK_LSF_PRINTLN(clid << " atry [p=" << p << "]");
                    if(verbose) vars.display(std::cerr,atry,"\tatry.");
                    YACK_LSF_PRINTLN("D2try = " << D2try);


                    //----------------------------------------------------------
                    //
                    // check resulting D2
                    //
                    //----------------------------------------------------------
                    if(D2try<=D2ini)
                    {
                        //------------------------------------------------------
                        //
                        // success
                        //
                        //------------------------------------------------------
                        if(!shrinking)
                        {
                            // allowed to explore more
                            D2try = this->optimize(g,D2ini,D2try);
                        }

                        //------------------------------------------------------
                        //
                        // decreased
                        //
                        //------------------------------------------------------
                        tao::v1::set(aorg,atry);
                        D2 = D2try;
                        if(shrinking)
                        {
                            YACK_LSF_PRINTLN(clid << " would process again");
                            return cycle_process;
                        }
                        else
                        {
                            YACK_LSF_PRINTLN(clid << " consistent cycle");
                            p = max_of(--p,this->pmin);
                            return cycle_success;
                        }
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // increase of D2 : shrink and recompute curvature
                        //
                        //------------------------------------------------------
                        if(!this->shrink(p))
                        {
                            YACK_LSF_PRINTLN(clid << " unable to decrease!!");
                            return cycle_failure;
                        }
                        goto CURV;
                    }

                }

                //______________________________________________________________
                //
                //! wrapper to cycle
                //______________________________________________________________
                template <typename FUNC>
                cycle_result cycle_(sample_type              &s,
                                    unit_t                   &p,
                                    FUNC                     &func,
                                    writable<ORDINATE>       &aorg,
                                    const readable<bool>     &used,
                                    const readable<ORDINATE> &scal,
                                    control                 *proc)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> F(func);
                    return cycle(s,p,F,aorg,used,scal,proc);
                }

                //______________________________________________________________
                //
                //! fit session
                //______________________________________________________________
                inline bool fit_(sample_type              &s,
                                 sequential_type          &func,
                                 writable<ORDINATE>       &aorg,
                                 const readable<bool>     &used,
                                 const readable<ORDINATE> &scal,
                                 control                 *proc)
                {
                    //----------------------------------------------------------
                    //
                    // initialize
                    //
                    //----------------------------------------------------------
                    unit_t p  = 0; // power indicator
                    size_t c  = 1; // cycle indicator
                    YACK_LSF_PRINTLN(clid << " -------- @cycle #" << c << " --------");


                    //----------------------------------------------------------
                    //
                    // first step
                    //
                    //----------------------------------------------------------
                    switch(cycle(s,p,func,aorg,used,scal,proc))
                    {
                            //--------------------------------------------------
                            // error/singular
                            //--------------------------------------------------
                        case cycle_failure:
                            YACK_LSF_PRINTLN(clid << " singular system");
                            return false;

                            //--------------------------------------------------
                            // not finished
                            //--------------------------------------------------
                        case cycle_process: break;

                            //--------------------------------------------------
                            // successful, test variables
                            //--------------------------------------------------
                        case cycle_success:
                            if(this->converged(aorg))
                            {
                                YACK_LSF_PRINTLN(clid << " converged variables @cycle #" << c);
                                return true;
                            }
                            break;
                    }

                    ORDINATE D2old = D2;



                    //----------------------------------------------------------
                    //
                    // loop
                    //
                    //----------------------------------------------------------
                LOOP:
                    ++c;
                    YACK_LSF_PRINTLN(clid << " -------- @cycle #" << c << " --------");
                    switch(cycle(s,p,func,aorg,used,scal,proc))
                    {
                            //--------------------------------------------------
                            //
                            // singularity
                            //
                            //--------------------------------------------------
                        case cycle_failure: return false;


                            //--------------------------------------------------
                            //
                            // successful step
                            //
                            //--------------------------------------------------
                        case cycle_success:
                            //--------------------------------------------------
                            // test convergence on variable
                            //--------------------------------------------------
                            if(this->converged(aorg))
                            {
                                YACK_LSF_PRINTLN(clid << " converged parameters @cycle #" << c);
                                return true;
                            }

                            //--------------------------------------------------
                            // test convergence on D
                            //--------------------------------------------------
                            if(fabs(D2old-this->D2) <= this->dtol * D2old )
                            {
                                YACK_LSF_PRINTLN(clid << " converged D2=" << D2 << " @cycle #" << c);
                                return true;
                            }

                            //--------------------------------------------------
                            // update D2old
                            //--------------------------------------------------
                            D2old =  D2;
                            goto LOOP;

                            //--------------------------------------------------
                            //
                            // not settled yet
                            //
                            //--------------------------------------------------
                        case cycle_process:
                            goto LOOP;
                    }


                    return true;
                }

                //______________________________________________________________
                //
                //! fit session wrapper
                //______________________________________________________________
                template <typename FUNC> inline
                bool fit(sample_type              &s,
                         FUNC                     &func,
                         writable<ORDINATE>       &aorg,
                         const readable<bool>     &used,
                         const readable<ORDINATE> &scal,
                         control                  *proc)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> F(func);
                    return fit_(s,F,aorg,used,scal,proc);
                }

                //______________________________________________________________
                //
                //! per-parameter error computation
                //______________________________________________________________
                inline ORDINATE errors_(writable<ORDINATE>       &aerr,
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
                inline ORDINATE errors(writable<ORDINATE>       &aerr,
                                       sample_type              &s,
                                       FUNC                     &func,
                                       const readable<ORDINATE> &aorg,
                                       const readable<bool>     &used,
                                       const readable<ORDINATE> &scal)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> F(func);
                    return errors_(aerr,s,F,aorg,used,scal);
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

