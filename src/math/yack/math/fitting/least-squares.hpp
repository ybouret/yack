
//! \file

#ifndef YACK_FIT_LEAST_SQUARES_INCLUDED
#define YACK_FIT_LEAST_SQUARES_INCLUDED 1

#include "yack/math/fitting/sample.hpp"
#include "yack/math/fitting/lambda.hpp"
#include "yack/math/fitting/least-squares/base.hpp"
#include "yack/math/algebra/lss.hpp"
#include "yack/math/iota.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/type/temporary.hpp"
#include "yack/math/numeric.hpp"
#include "yack/math/opt/optimize.hpp"
#include <iomanip>

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //! helper for verbose output
#define YACK_LSF_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

            //! least squares algorithm
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
                typedef lss<ORDINATE>                         solver;          //!< alias
                typedef derivative<ORDINATE>                  drvs_type;       //!< alias
                typedef typename drvs_type::pointer           drvs_ptr;        //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~least_squares() throw() {}


                //! setup
                explicit least_squares(const drvs_ptr *ppDrvs = NULL) :
                curv(),
                tabs(8,0),
                curr(NULL),
                hfcn(NULL),
                aorg( tabs.next() ),
                step( tabs.next() ),
                aend( tabs.next() ),
                atry( tabs.next() ),
                solv(true),
                drvs(  ppDrvs ? & coerce(**ppDrvs) : new drvs_type() ),
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
                //! mae atry
                //______________________________________________________________
                void   make_atry(const double u) throw()
                {
                    for(const vnode *node= (**curr).head();node;node=node->next)
                    {
                        const size_t i = ****node;
                        atry[i] = aorg[i] + u * step[i];
                    }
                }

                //______________________________________________________________
                //
                //! callable wrapper
                //______________________________________________________________
                double operator()(const double u)
                {
                    make_atry(u);
                    return curr->D2(*hfcn,atry);
                }

                //! compute step from curvature and gradient
                inline void computeStep()
                {
                    iota::load(step,curr->beta);
                    solv.solve(curv,step);
                    iota::add(aend,aorg,step);
                }

                //! compute modified curvature
                inline bool computeCurv(const ORDINATE coef)
                {
                    assert(NULL!=curr);
                    const ORDINATE fac = ORDINATE(1) + coef;
                    curv.assign(curr->curv);
                    for(size_t i=curv.rows;i>0;--i)
                        curv[i][i] *= fac;
                    return solv.build(curv);
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
                    static const ORDINATE xtol = lam[ lam.ptol() ];
                    //----------------------------------------------------------
                    //
                    //
                    // initialize session
                    //
                    //
                    //----------------------------------------------------------
                    YACK_LSF_PRINTLN(clid << "[start session]");

                    //----------------------------------------------------------
                    // check variables
                    //----------------------------------------------------------
                    const variables & vars = *s;
                    const size_t      nvar = vars.upper();
                    if(nvar<=0)
                    {
                        YACK_LSF_PRINTLN(clid << "[no variables]");
                        return false;
                    }

                    //----------------------------------------------------------
                    // initialize errors
                    //----------------------------------------------------------
                    vars.ldz(aerr);

                    //----------------------------------------------------------
                    // check d.o.f.
                    //----------------------------------------------------------
                    const size_t ndat = s.dimension();
                    if(nvar>ndat)
                    {
                        (void) s.D2(f,a0);
                        YACK_LSF_PRINTLN(clid << "[not enough data]");
                        return false;
                    }

                    //----------------------------------------------------------
                    // get memory
                    //----------------------------------------------------------
                    curv.make(nvar,nvar);
                    tabs.make(nvar);
                    solv.ensure(nvar);

                    //----------------------------------------------------------
                    // temporary internal values
                    //----------------------------------------------------------
                    const temporary<sample_type*>     tmpS(curr,&s);
                    const temporary<sequential_type*> tmpF(hfcn,&f);

                    //----------------------------------------------------------
                    // set parameters parameters
                    //----------------------------------------------------------
                    lam.initialize(p10);
                    vars.mov(aorg,a0);

                    if(verbose){
                        std::cerr << clid << "[initialized]" << std::endl;
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
                    ++cycle;
                    YACK_LSF_PRINTLN(clid << "-------- cycle #" << cycle <<" --------");
                    YACK_LSF_PRINTLN(clid << "D2_org = " << D2_org);
                    YACK_LSF_PRINTLN(clid << "lambda = " << lam[p10]);

                    //----------------------------------------------------------
                    //
                    // compute curvature with regularization
                    //
                    //----------------------------------------------------------
                GUESS:
                    while(!computeCurv(lam[p10]))
                    {
                        if(!lam.increase(p10)) {
                            YACK_LSF_PRINTLN(clid << "<singular>]");
                            return false;
                        }
                        YACK_LSF_PRINTLN(clid << "lambda = " << lam[p10]);
                    }

                    //----------------------------------------------------------
                    //
                    // compute
                    //
                    //----------------------------------------------------------
                    computeStep();

                    ORDINATE D2_end = s.D2(f,aend);
                    YACK_LSF_PRINTLN(clid << "D2_end = " << D2_end << " / " << D2_org);

                    {
                        const string  fn = "D2-" + s.name + ".dat";
                        ios::ocstream fp(fn);
                        const size_t  np = 100;
                        for(size_t i=0;i<=np;++i)
                        {
                            const double u = i/double(np);
                            const double g = double( (*this)(u) );
                            fp("%g %.15g\n",u,g);
                        }
                    }

                    if(D2_end <= D2_org )
                    {
                        YACK_LSF_PRINTLN(clid << "[accept]");
                        study(D2_org,D2_end);
                        exit(0);
                    }
                    else
                    {
                        YACK_LSF_PRINTLN(clid << "[recject]]");
                        assert(D2_end>D2_org);
                        if(!lam.increase(p10))
                        {
                            YACK_LSF_PRINTLN(clid << "<spurious>");
                            return false;
                        }
                        exit(0);
                        goto GUESS;
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
                drvs_ptr                 drvs;
                int                      p10;
                const lambda<ORDINATE>   lam;
            public:
                bool                     verbose; //!< verbosity

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares);




                //______________________________________________________________
                //
                //! check the possibility of an overshoot
                //______________________________________________________________
                void study(const ORDINATE D2_org,
                           ORDINATE      &D2_end)
                {
                    static const ORDINATE half(0.5);
                    assert(D2_end<=D2_org);
                    const ORDINATE D2_mid = (*this)(half);
                    const ORDINATE beta   = twice( solv.xadd(D2_org,D2_end, -(D2_mid+D2_mid) ) );
                    std::cerr << "\tbeta  = " << beta  << std::endl;
                    if(beta>0)
                    {
                        const ORDINATE alpha  = solv.xadd( 4*D2_mid, -3*D2_org, -D2_end);
                        std::cerr << "\talpha = " << alpha << std::endl;
                        if(alpha<0)
                        {
                            const ORDINATE num = -alpha;
                            const ORDINATE den = beta+beta;
                            std::cerr << "\tu_opt = " << num/den << std::endl;
                        }
                    }

                }


            };

        }

    }

}

#endif
