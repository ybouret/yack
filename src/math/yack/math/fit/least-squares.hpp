//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fit/sample.hpp"
#include "yack/math/fit/lambda.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {

            enum cycle_result
            {
                cycle_success,
                cycle_process,
                cycle_failure
            };

            template <
            typename ABSCISSA,
            typename ORDINATE>
            class least_squares : public large_object, public arrays_of<ORDINATE>
            {
            public:
                typedef sample<ABSCISSA,ORDINATE>                sample_type;
                typedef arrays_of<ORDINATE>                      tableaux;
                typedef typename arrays_of<ORDINATE>::array_type array_type;
                typedef sequential<ABSCISSA,ORDINATE>            sequential_type;
                typedef lu<ORDINATE>                             lu_type;
                typedef auto_ptr<lu_type>                        algo_type;
                typedef functor<bool,TL1(writable<ORDINATE>&)>   callback; //!< true->ok, false->bad



                inline virtual ~least_squares() throw() {}
                inline explicit least_squares() :
                large_object(),
                tableaux(4,0),
                D2(0),
                step( this->next() ),
                atry( this->next() ),
                curv(),
                drvs(),
                algo(),
                lam(),
                pmin( lam.lower ),
                pmax( lam.upper ),
                ftol( lam[pmin+1] ),
                dtol( lam[pmin/2] ),
                shrinking(false),
                verbose(false)
                {
                }

                cycle_result cycle(sample_type              &s,
                                   unit_t                   &p,
                                   sequential_type          &func,
                                   writable<ORDINATE>       &aorg,
                                   const readable<bool>     &used,
                                   const readable<ORDINATE> &scal,
                                   callback                 *proc)
                {
                    assert(aorg.size()==used.size());
                    assert(aorg.size()==scal.size());

                    //----------------------------------------------------------
                    //
                    // initialize local memory
                    //
                    //----------------------------------------------------------
                    const size_t   np = aorg.size(); initialize(np);
                    p = clamp(pmin,p,pmax);

                    //----------------------------------------------------------
                    //
                    // compute all metrics @aorg
                    //
                    //----------------------------------------------------------
                    const ORDINATE D2ini = D2 = s.D2_full(func,aorg,used,drvs,scal);
                    s.finalize();

                    if(verbose)
                    {
                        std::cerr << "[LS] cycle @p=" << p << " => lambda=" << lam[p] << std::endl;;
                        std::cerr << "aorg  = " << aorg << std::endl;
                        (*s).display(std::cerr,aorg,"\taorg.");
                        std::cerr << "D2ini = " << D2ini << std::endl;
                        std::cerr << "curv  = " << s.curv << std::endl;
                        std::cerr << "beta  = " << s.beta << std::endl;
                    }


                CURV:
                    //----------------------------------------------------------
                    //
                    // try compute curvature
                    //
                    //----------------------------------------------------------
                    if(!compute_curvature(s,p,used))
                    {
                        //------------------------------------------------------
                        // singular
                        //------------------------------------------------------
                        if(verbose) std::cerr << "[LS] singular parameters" << std::endl;
                        return cycle_failure;
                    }

                    //----------------------------------------------------------
                    //
                    // compute step
                    //
                    //----------------------------------------------------------
                    tao::v1::load(step,s.beta);
                    algo->solve(curv,step);
                    if(verbose)
                    {
                        std::cerr << "[LS] step @p=" << p << std::endl;
                        std::cerr << "step  = " << step << std::endl;
                        (*s).display(std::cerr,step,"\tstep.");
                    }

                    //----------------------------------------------------------
                    //
                    // compute trial parameters
                    //
                    //----------------------------------------------------------
                    tao::v1::add(atry,aorg,step);             // atry=aorg+step
                    if(proc)                                  // post process
                    {

                        if( !(*proc)(atry) )
                        {
                            if(verbose) std::cerr << "[LS] post-processing warning" << std::endl;
                            // something happened
                            if(!shrink(p))
                            {
                                if(verbose) std::cerr << "[LS] post-processing caused failure" << std::endl;
                                return cycle_failure;
                            }
                        }
                    }

                    tao::v1::sub(step,atry,aorg);             // recompute step in any case
                    const ORDINATE D2try = s.D2(func,atry);   // compute trial value
                    if(verbose)
                    {
                        std::cerr << "atry  = " << atry << std::endl;
                        (*s).display(std::cerr,atry,"\tatry.");
                        std::cerr << "D2try = " << D2try << std::endl;
                    }

                    if(D2try<=D2ini)
                    {
                        //------------------------------------------------------
                        //
                        // decreased
                        //
                        //------------------------------------------------------
                        tao::v1::set(aorg,atry);
                        D2 = D2try;
                        if(shrinking)
                        {
                            if(verbose) std::cerr << "should process again" << std::endl;
                            return cycle_process;
                        }
                        else
                        {
                            if(verbose) std::cerr << "successful step" << std::endl;
                            p = max_of(--p,pmin);
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
                        if(!shrink(p))
                        {
                            return cycle_failure;
                        }
                        goto CURV;
                    }

                }

                template <typename FUNC>
                cycle_result cycle_(sample_type              &s,
                                    unit_t                   &p,
                                    FUNC                     &func,
                                    writable<ORDINATE>       &aorg,
                                    const readable<bool>     &used,
                                    const readable<ORDINATE> &scal,
                                    callback                 *proc)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> F(func);
                    return cycle(s,p,F,aorg,used,scal,proc);
                }

                inline bool fit(sample_type              &s,
                                sequential_type          &func,
                                writable<ORDINATE>       &aorg,
                                const readable<bool>     &used,
                                const readable<ORDINATE> &scal,
                                callback                 *proc)
                {
                    //----------------------------------------------------------
                    //
                    // initialize
                    //
                    //----------------------------------------------------------
                    unit_t p  = 0;
                    size_t c  = 1;
                    if(verbose) std::cerr << "[LS] @cycle #" << c << std::endl;

                    //----------------------------------------------------------
                    //
                    // first step
                    //
                    //----------------------------------------------------------
                    switch(cycle(s,p,func,aorg,used,scal,proc))
                    {
                        case cycle_failure: return false; // error/singular
                        case cycle_process: break;        // not finished
                        case cycle_success:               // successful, test variables
                            if(converged(aorg))
                            {
                                if(verbose) std::cerr << "[LS] converged variables" << std::endl;
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
                    if(verbose) std::cerr << "[LS] @cycle #" << c << std::endl;
                    switch(cycle(s,p,func,aorg,used,scal,proc))
                    {
                            //--------------------------------------------------
                            // singularity
                            //--------------------------------------------------
                        case cycle_failure: return false;

                        case cycle_success:
                            //--------------------------------------------------
                            // test convergence on variable
                            //--------------------------------------------------
                            if(converged(aorg))
                            {
                                if(verbose) std::cerr << "[LS] converged parameters" << std::endl;
                                return true;
                            }

                            //--------------------------------------------------
                            // test convergence on D
                            //--------------------------------------------------
                            if(fabs(D2old-D2) <= dtol * D2old )
                            {
                                if(verbose) std::cerr << "[LS] converged D2" << std::endl;
                                return true;
                            }

                            D2old = D2;
                            goto LOOP;

                        case cycle_process:
                            goto LOOP;
                    }


                    return true;
                }

                template <typename FUNC> inline
                bool fit_(sample_type              &s,
                          FUNC                     &func,
                          writable<ORDINATE>       &aorg,
                          const readable<bool>     &used,
                          const readable<ORDINATE> &scal,
                          callback                 *proc)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> F(func);
                    return fit(s,F,aorg,used,scal,proc);
                }



                ORDINATE               D2;   //!< last D2
                array_type            &step; //!< last step
                array_type            &atry; //!< trial parameters
                matrix<ORDINATE>       curv; //!< modified matrix
                derivative<ORDINATE>   drvs; //!< derivative
                algo_type              algo; //!< algebra
                const lambda<ORDINATE> lam;  //!< pre-computed coefficients
                const unit_t           pmin;
                const unit_t           pmax;
                const ORDINATE         ftol; //!< lam[pmin+1]
                const ORDINATE         dtol; //!< lam[pmin/2]

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);
                bool shrinking; //!< internal flag per cycle

                bool converged(const readable<ORDINATE> &atmp) const throw()
                {
                    assert( atmp.size() == step.size() );

                    for(size_t i=atmp.size();i>0;--i)
                    {
                        const ORDINATE da = fabs( step[i] );
                        const ORDINATE aa = fabs( atmp[i] );
                        if( da > ftol * aa ) return false;
                    }


                    return true;
                }

                void initialize(const size_t npar)
                {
                    this->make(npar);
                    curv.make(npar,npar);
                    shrinking = false;
                    if(algo.is_empty()||algo->dims<npar)
                    {
                        algo = new lu_type(npar);
                    }
                }

                bool shrink(unit_t &p) throw()
                {
                    shrinking = true;
                    return ++p <= pmax;
                }

                bool compute_curvature(const sample_type    &s,
                                       unit_t               &p,
                                       const readable<bool> &used)
                {
                    assert(p>=lam.lower);
                    assert(p<=lam.upper);
                    static const ORDINATE _1(1);
                    const size_t          npar = used.size();
                    const variables      &vars = *s;

                TRY:
                    const ORDINATE        dfac = _1 +lam[p];
                    curv.assign(s.curv);
                    for(size_t i=npar;i>0;--i)
                    {
                        if(!used[i]||!vars.handles(i))
                        {
                            curv[i][i] = _1;
                        }
                        else
                        {
                            curv[i][i] *= dfac;
                        }
                    }


                    if(!algo->build(curv))
                    {
                        if(!shrink(p)) return false; // singular
                        goto TRY;
                    }


                    return true;

                }

            public:
                bool verbose; //!< verbosity

            };

        }
    }

}

#endif

