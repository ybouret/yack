
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

            template <typename ABSCISSA,typename ORDINATE>
            class least_squares
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
                p10(0),
                lam()
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
                    if(solv.is_empty()||solv->nmax<nvar) solv = new LU(nvar);

                    if(ndat<=0) return false;
                    if(nvar<=0) return false;

                    ORDINATE D2_org = s.D2_full(f,aorg, used, scal, *drvs);


                    if(!compute_step(s))
                    {
                        // singular
                        return false;
                    }

                    vars(std::cerr,aorg,NULL)    << std::endl;
                    vars(std::cerr,step,"step_") << std::endl;



                    return false;
                }



                matrix<ORDINATE>         curv;
                ordinates                beta;
                ordinates                step;
                solver                   solv;
                drvs_t                   drvs;
                int                      p10;
                const lambda<ORDINATE>   lam;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);

                inline bool compute_step(const sample_type &s) throw()
                {
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
                            goto TRY_AGAIN;
                        }
                        else
                        {
                            // singular point
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
