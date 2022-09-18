
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
#include "yack/sort/network/sort4.hpp"

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


                    //----------------------------------------------------------
                    //
                    //
                    // CYCLE
                    //
                    //
                    //----------------------------------------------------------
                    ios::ocstream::overwrite("D2_" + s.name + ".log");
                    ios::ocstream::echo("D2_" + s.name + ".log","%u %.15g %.15g\n", unsigned(cycle), D2_org, 1);
              
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
                    // compute step and aend
                    //
                    //----------------------------------------------------------
                    computeStep();
                    vars(std::cerr << "aend=",aend,NULL)    << std::endl;
                    vars(std::cerr << "step=",step,NULL)    << std::endl;


                    ORDINATE D2_end = s.D2(f,aend);
                    YACK_LSF_PRINTLN(clid << "D2_end = " << D2_end << " / " << D2_org);

                    if(true)
                    {
                        const string  fn = "D2-" + s.name + ".dat";
                        ios::ocstream fp(fn);
                        const size_t  np = 100;
                        for(size_t i=0;i<=np+50;++i)
                        {
                            const double u = i/double(np);
                            const double g = double( (*this)(u) );
                            fp("%g %.15g\n",u,g);
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // study result
                    //
                    //----------------------------------------------------------
                    if(D2_end <= D2_org )
                    {
                        //------------------------------------------------------
                        //
                        YACK_LSF_PRINTLN(clid << "[accept]");
                        //
                        //------------------------------------------------------
                        analyze(D2_org,D2_end);
                        ios::ocstream::echo("D2_" + s.name + ".log","%u %.15g %.15g\n", unsigned(cycle), D2_end, (D2_org-D2_end)/D2_end);

                        //------------------------------------------------------
                        // check D2 convergence
                        //------------------------------------------------------
                        assert(D2_end<=D2_org);
                        {
                            const ORDINATE delta = std::abs(D2_org-D2_end);
                            const ORDINATE limit = xtol * D2_end;
                            std::cerr << "delta=" << delta << "/" << limit << std::endl;
                        }

                        //------------------------------------------------------
                        // check variable convergence
                        //------------------------------------------------------
                        YACK_LSF_PRINTLN(clid << "[convergence]");
                        bool converged = true;
                        for(const vnode *node=vars.head();node;node=node->next)
                        {
                            const variable &v     = ***node;
                            const size_t    i     = *v; if(!used[i]) continue;
                            const ORDINATE  a_old = aorg[i];
                            const ORDINATE  a_new = aend[i];
                            const ORDINATE  delta = std::abs(a_old - a_new);
                            const ORDINATE  limit = xtol * max_of( std::abs(a_old), std::abs(a_new) );
                            const bool      is_ok = delta <= limit;
                            if(!is_ok)
                            {
                                converged = false;
                            }
                            vars.pad(std::cerr << ok(is_ok) << v.name, v.name()) << " : ";
                            std::cerr << std::setw(15) << a_old << " -> " << std::setw(15) << a_new;
                            std::cerr << " [" << std::setw(15) << delta << "/" << limit << "]";
                            std::cerr << std::endl;
                        }

                        if(converged)
                        {
                            goto SUCCESS;
                        }

                        vars.mov(aorg,aend);
                        D2_org = s.D2_full(f,aorg, used, scal, *drvs);
                        lam.decrease(p10);
                        goto CYCLE;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        YACK_LSF_PRINTLN(clid << "[reject]");
                        //
                        //------------------------------------------------------
                        assert(D2_end>D2_org);
                        if(!lam.increase(p10))
                        {
                            YACK_LSF_PRINTLN(clid << "<spurious>");
                            return false;
                        }
                        YACK_LSF_PRINTLN(clid << "lambda = " << lam[p10]);
                        goto GUESS;
                    }

                SUCCESS:
                    YACK_LSF_PRINTLN(clid << "[converged!]");

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
                //
                //! analyze linear step
                //
                //______________________________________________________________
                inline void analyze(const ORDINATE D2_org,
                                    ORDINATE      &D2_end)
                {
                    //__________________________________________________________
                    //
                    // linear curvature
                    //__________________________________________________________
                    static const ORDINATE half(0.5);
                    static const ORDINATE umax(1.1);
                    static const ORDINATE ures(0.01);

                    assert(D2_end<=D2_org);
                    const ORDINATE D2_mid = (*this)(half);
                    const ORDINATE beta   = twice( solv.xadd(D2_org,D2_end, -(D2_mid+D2_mid) ) );
                    YACK_LSF_PRINTLN(clid << "beta  = " << beta);
                    if(beta<=0) return;

                    const ORDINATE slope = solv.xadd.dot(curr->beta,step);
                    std::cerr << "slope=" << slope << std::endl;

                    exit(0);
                    
                    //__________________________________________________________
                    //
                    // linear slope
                    //__________________________________________________________
                    const ORDINATE alpha  = solv.xadd( 4*D2_mid, -3*D2_org, -D2_end);
                    YACK_LSF_PRINTLN(clid << "alpha = " << alpha);
                    //std::cerr << "plot 'D2-" << curr->name << ".dat' w p, " << D2_org << " +(" << alpha << ")*x + (" << beta << ") *x*x" << std::endl;
                    if(alpha>=0) return;

                    //__________________________________________________________
                    //
                    // linear optimum
                    //__________________________________________________________
                    const ORDINATE num   = -alpha;
                    const ORDINATE den   = beta+beta;

                    const ORDINATE u_opt  = num <= umax * den ? num/den : umax;
                    const ORDINATE D2_opt = (*this)(u_opt);
                    YACK_LSF_PRINTLN(clid << "u_opt  = " << u_opt);
                    YACK_LSF_PRINTLN(clid << "D2_opt = " << D2_opt);

                    ORDINATE u[4] = { 0,      0.5,    1,      u_opt  };
                    ORDINATE h[4] = { D2_org, D2_mid, D2_end, D2_opt };

                    network_sort4 srt;
                    srt.csort(u,h); assert(u[0]<=u[1]); assert(u[1]<=u[2]); assert(u[2]<=u[3]);

                    //__________________________________________________________
                    //
                    // locate global after u[0]
                    //__________________________________________________________
                    size_t   imin=1;
                    ORDINATE hmin=h[1];
                    for(size_t i=2;i<4;++i)
                    {
                        const ORDINATE tmp = h[i];
                        if(tmp<=hmin)
                        {
                            hmin = tmp;
                            imin = i;
                        }
                    }


                    if(imin==3)
                    {
                        //______________________________________________________
                        //
                        // global descent
                        //______________________________________________________
                        make_atry(u[3]);
                        (**curr).mov(aend,atry);
                        D2_end = h[3];
                        return;
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // local tightening
                        //______________________________________________________
                        assert(imin==1||imin==2);
                        const size_t ia = imin-1;
                        const size_t ib = ia+1;
                        const size_t ic = ia+2;

                        triplet<ORDINATE> U = { u[ia], u[ib], u[ic] };
                        triplet<ORDINATE> H = { h[ia], h[ib], h[ic] };

                        while(true)
                        {
                            const ORDINATE w = optimize::tighten_for(*this,U,H);
                            YACK_LSF_PRINTLN(clid << "[tightening] U=" << U << ", H=" << H << " @" <<w);
                            if(w<=ures)
                                break;
                        }

                        //______________________________________________________
                        //
                        // finalize and return to detect convergence
                        //______________________________________________________
                        make_atry(U.b);
                        (**curr).mov(aend,atry);
                        D2_end = H.b;
                    }
                }


            };

        }

    }

}

#endif
