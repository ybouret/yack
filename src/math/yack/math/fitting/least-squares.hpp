
//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fitting/sample.hpp"
#include "yack/math/fitting/lambda.hpp"
#include "yack/math/fitting/least-squares/base.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/type/temporary.hpp"
#include "yack/math/numeric.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //! helper for verbose output
#define YACK_LSF_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

            //! least_squares algorithm
            template <typename ABSCISSA,typename ORDINATE>
            class least_squares : public least_squares_
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                typedef sample<ABSCISSA,ORDINATE>             sample_type;     //!< alias
                typedef arrays_of<ORDINATE>                   tableaux;        //!< alias
                typedef typename tableaux::array_type         array_type;      //!< alias
                typedef typename sample_type::sequential_type sequential_type; //!< alias
                typedef crout<ORDINATE>                       LU;              //!< alias
                typedef auto_ptr<LU>                          solver;          //!< alias
                typedef auto_ptr< derivative<ORDINATE> >      drvs_t;          //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~least_squares() throw() {}


                //! setup
                explicit least_squares() :
                curv(),
                tabs(8,0),
                curr(NULL),
                hfcn(NULL),
                aorg( tabs.next() ),
                step( tabs.next() ),
                aend( tabs.next() ),
                atry( tabs.next() ),
                solv(NULL),
                drvs( new derivative<ORDINATE>() ),
                p10(0),
                lam(),
                verbose(false)
                {
                }

                //______________________________________________________________
                //
                //! fit wrapper
                //______________________________________________________________
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

                //______________________________________________________________
                //
                //! callable wrapper
                //______________________________________________________________
                double operator()(const double u)
                {
                    for(size_t i=aorg.size();i>0;--i)
                    {
                        atry[i] = aorg[i] + u * step[i];
                    }
                    return curr->D2(*hfcn,atry);
                }

                //______________________________________________________________
                //
                //! fit a sample
                //______________________________________________________________
                bool fit(sample_type              &s,
                         sequential_type          &f,
                         writable<ORDINATE>       &a0,
                         const readable<bool>     &used,
                         const readable<ORDINATE> &scal,
                         writable<ORDINATE>       &aerr)
                {

                    //----------------------------------------------------------
                    //
                    //
                    // initialize session
                    //
                    //
                    //----------------------------------------------------------
                    //static const ORDINATE             tol = numeric<ORDINATE>::ftol;
                    const size_t                      ndat = s.dimension();
                    const variables &                 vars = *s;
                    const size_t                      nvar = vars.upper();
                    const temporary<sample_type*>     tmpS(curr,&s);
                    const temporary<sequential_type*> tmpF(hfcn,&f);

                    lam.initialize(p10);
                    curv.make(nvar,nvar);
                    tabs.make(nvar);
                    vars.mov(aorg,a0);
                    vars.ldz(aerr);



                    if(solv.is_empty()||solv->nmax<nvar) solv = new LU(nvar);

                    if(ndat<=0) return false;
                    if(nvar<=0) return false;

                    if(verbose){
                        std::cerr << clid << "initialize" << std::endl;
                        vars(std::cerr << "aorg=",aorg,NULL)    << std::endl;
                    }

                    //----------------------------------------------------------
                    //
                    //
                    // starting point
                    //
                    //
                    //----------------------------------------------------------
                    size_t   cycle  = 0;
                    ORDINATE D2_org = s.D2_full(f,aorg, used, scal, *drvs);
                CYCLE:
                    YACK_LSF_PRINTLN("D2_org=" << D2_org);
                    ++cycle;

                    //----------------------------------------------------------
                    //
                    //
                    // try to predict a step
                    //
                    //
                    //----------------------------------------------------------
                PREDICT:
                    if(!predict())
                        return false;

                    //----------------------------------------------------------
                    //
                    //
                    // compute queried position
                    //
                    //
                    //----------------------------------------------------------
                    const ORDINATE D2_end = s.D2(f,aend);

                    if(verbose) {
                        vars(std::cerr << "step=",step,"step_") << std::endl;
                        vars(std::cerr << "aend=",aend,NULL)    << std::endl;
                        std::cerr << "  D2_end = " << D2_end << "/" << D2_org << std::endl;
                    }

                    //----------------------------------------------------------
                    //
                    //
                    // check results
                    //
                    //
                    //----------------------------------------------------------
                    if(D2_end<D2_org)
                    {
                        //------------------------------------------------------
                        //
                        // accept
                        //
                        //------------------------------------------------------
                        YACK_LSF_PRINTLN(clid << "[accept]");

                        if(true)
                        {
                            ios::ocstream fp("decreased.dat");
                            const size_t NP=100;
                            for(size_t i=0;i<=NP;++i)
                            {
                                const double u = i/double(NP);
                                fp("%g %g\n",u,(*this)(u));
                            }

                            std::cerr << "slope=" << s.xadd.dot(s.beta,step) << std::endl;

                            exit(1);
                        }



                        iota::load(aorg,aend);
                        D2_org = s.D2_full(f,aorg, used, scal, *drvs);


                        goto CYCLE;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // reject
                        //
                        //------------------------------------------------------
                        YACK_LSF_PRINTLN(clid << "[reject]");
                        if(!lam.increase(p10))
                        {
                            YACK_LSF_PRINTLN(clid << "[spurious]");
                            return false;
                        }
                        goto PREDICT;
                    }


                    return true;
                }


            private:
                matrix<ORDINATE>         curv;
                tableaux                 tabs;
                sample_type             *curr;
                sequential_type         *hfcn;
                array_type &             aorg;
                array_type &             step;
                array_type &             aend;
                array_type &             atry;
                solver                   solv;
                drvs_t                   drvs;
                int                      p10;
                const lambda<ORDINATE>   lam;
            public:
                bool                     verbose; //!< verbosity

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);

                inline static
                bool converged(const ORDINATE old_v,
                               const ORDINATE new_v,
                               const ORDINATE tol) throw()
                {
                    const ORDINATE delta = std::abs(old_v-new_v);
                    const ORDINATE limit = tol * max_of( std::abs(old_v), std::abs(new_v) );
                    std::cerr << "delta=" << delta << " / " << limit << std::endl;
                    return (delta<=limit);
                }


                inline bool converged(const readable<bool> &used, const ORDINATE tol) const throw()
                {
                    for(const vnode *node= (**curr).head(); node;node=node->next)
                    {
                        const size_t   i     = ****node; if(!used[i]) continue;
                        if(!converged(aorg[i],aend[i],tol)) return false;
                    }
                    return true;
                }

                inline bool predict() throw()
                {
                    YACK_LSF_PRINTLN(clid << "[predict]");
                    assert(NULL!=curr);
                TRY_AGAIN:
                    YACK_LSF_PRINTLN(clid << "[10^" << p10 << "=>" << lam[p10] << "]");
                    const ORDINATE fac = ORDINATE(1) + lam[p10];
                    curv.assign(curr->curv);
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
                            goto TRY_AGAIN;
                        }
                        else
                        {
                            // singular point
                            YACK_LSF_PRINTLN(clid << "[singular]");
                            return false;
                        }
                    }

                    iota::load(step,curr->beta);
                    solv->solve(curv,step);
                    iota::add(aend,aorg,step);
                    return true;
                }

            };

        }

    }

}

#endif
