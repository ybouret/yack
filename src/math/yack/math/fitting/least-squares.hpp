
//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fitting/sample.hpp"
#include "yack/math/fitting/lambda.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            class least_squares_
            {
            public:
                static const char clid[]; //!< "[least-squares]"

                virtual ~least_squares_() throw() {}
                explicit least_squares_() throw() {}


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares_);
            };

            const char least_squares_:: clid[] = "[least-squares] ";


#define YACK_LSF_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

            template <typename ABSCISSA,typename ORDINATE>
            class least_squares : public least_squares_
            {
            public:
                typedef sample<ABSCISSA,ORDINATE>             sample_type;
                typedef typename sample_type::ordinates       ordinates;
                typedef typename sample_type::sequential_type sequential_type;
                typedef crout<ORDINATE>                       LU;
                typedef auto_ptr<LU>                          solver;
                typedef auto_ptr< derivative<ORDINATE> >      drvs_t;

                virtual ~least_squares() throw() {}
                
                explicit least_squares() :
                curv(),
                beta(),
                step(),
                solv(NULL),
                drvs( new derivative<ORDINATE>() ),
                good(true),
                p10(0),
                lam(),
                verbose(false)
                {
                }

                template <typename FUNC>
                bool fit_with(FUNC                     &f,
                             sample_type              &s,
                             writable<ORDINATE>       &aorg,
                             const readable<bool>     &used,
                             const readable<ORDINATE> &scal,
                             writable<ORDINATE>       &aerr)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> call(f);
                    return fit(s,call,aorg,used,scal,aerr);
                }

                bool fit(sample_type              &s,
                         sequential_type          &f,
                         writable<ORDINATE>       &aorg,
                         const readable<bool>     &used,
                         const readable<ORDINATE> &scal,
                         writable<ORDINATE>       &aerr)
                {

                    // initialize session
                    const size_t     ndat = s.dimension();
                    const variables &vars = *s;
                    const size_t     nvar = vars.upper();
                    aerr.ld(0);
                    lam.initialize(p10);
                    curv.make(nvar,nvar);
                    beta.adjust(nvar,0);
                    step.adjust(nvar,0);
                    aend.adjust(nvar,0);
                    atry.adjust(nvar,0);
                    if(solv.is_empty()||solv->nmax<nvar) solv = new LU(nvar);

                    if(ndat<=0) return false;
                    if(nvar<=0) return false;

                    if(verbose){
                        std::cerr << clid << "initialize" << std::endl;
                        vars(std::cerr << "aorg=",aorg,NULL)    << std::endl;
                    }
                    
                    // starting point
                    ORDINATE D2_org = s.D2_full(f,aorg, used, scal, *drvs);
                    YACK_LSF_PRINTLN("D2_org=" << D2_org);
                    good = true;

                    // try to predict a step
                    if(!predict(s))
                        return false;

                    // compute queried position
                    iota::add(aend,aorg,step);

                    ORDINATE D2_end = s.D2(f,aend);

                    if(verbose) {
                        vars(std::cerr << "step=",step,"step_") << std::endl;
                        vars(std::cerr << "aend=",aend,NULL)    << std::endl;
                        std::cerr << "D2_end=" << D2_end << "/" << D2_org << std::endl;
                    }

                    // check positition
                    




                    return false;
                }



                matrix<ORDINATE>         curv;
                ordinates                beta;
                ordinates                step;
                ordinates                aend;
                ordinates                atry;
                solver                   solv;
                drvs_t                   drvs;
                bool                     good;
                int                      p10;
                const lambda<ORDINATE>   lam;
                bool                     verbose;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);

                inline bool predict(const sample_type &s) throw()
                {
                    YACK_LSF_PRINTLN(clid << "[predicting]");
                TRY_AGAIN:
                    const ORDINATE fac = ORDINATE(1) + lam[p10];
                    curv.assign(s.curv);
                    for(size_t i=curv.rows;i>0;--i)
                    {
                        curv[i][i] *= fac;
                    }

                    if(!solv->build(curv))
                    {
                        if( lam.increase(p10) )
                        {
                            // need to regularize
                            YACK_LSF_PRINTLN(clid << "[regularising]");
                            good = false;
                            goto TRY_AGAIN;
                        }
                        else
                        {
                            // singular point
                            YACK_LSF_PRINTLN(clid << "[singular]");
                            return false;
                        }
                    }

                    iota::load(step,s.beta);
                    solv->solve(curv,step);
                    return true;
                }

            };

        }

    }

}

#endif
