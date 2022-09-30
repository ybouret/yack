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
#include "yack/math/opt/locate.hpp"
#include "yack/math/data/percent.hpp"
#include "yack/functor.hpp"

#include <iomanip>

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            //__________________________________________________________________
            //
            //! helper for verbose output
            //__________________________________________________________________
#define YACK_LSF_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

            //__________________________________________________________________
            //
            //
            //! least squares algorithm
            //
            //__________________________________________________________________
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
                typedef TL3(sequential_type &,writable<ORDINATE>&,sample_type &) args_type; //!< alias
                typedef functor<bool,args_type>                                  process;   //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~least_squares() throw() {}


                //! setup
                explicit least_squares(const drvs_ptr *ppDrvs = NULL) :
                curv(),
                tabs(4,0),
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
                              writable<ORDINATE>       &aerr,
                              process                  *proc = NULL)
                {
                    typename sample_type:: template sequential_wrapper<FUNC> call(f);
                    return fit(s,call,aorg,used,scal,aerr,proc);
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
                    YACK_LSF_PRINTLN(clid << "[computing curvature @lambda=" << coef << "]" );
                    const ORDINATE fac = ORDINATE(1) + coef; // compute coeff
                    curv.assign(curr->curv);                 // load curvature
                    for(size_t i=curv.rows;i>0;--i)          // compute modified
                        curv[i][i] *= fac;
                    return solv.build(curv);                 // build it with solver
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
                         writable<ORDINATE>       &aerr,
                         process                  *proc)
                {

                    //----------------------------------------------------------
                    //
                    //
                    // initialize session
                    //
                    //
                    //----------------------------------------------------------
                    YACK_LSF_PRINTLN(clid << "[starting session]");

                    //----------------------------------------------------------
                    // check variables
                    //----------------------------------------------------------
                    const variables & vars = *s;
                    const size_t      nvar = vars.upper();
                    const size_t      ndat = s.dimension();

                    if(nvar<=0)
                    {
                        (void)s.D2(f,a0);
                        YACK_LSF_PRINTLN(clid << "<no variables!>");
                        return false;
                    }
                    vars.ldz(aerr);

                    if(ndat<=0)
                    {
                        YACK_LSF_PRINTLN(clid << "<no data!>");
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
                    // set parameters
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
                    // starting point with full step
                    //
                    //
                    //----------------------------------------------------------
                    unsigned              cycle  = 0;
                    ORDINATE              f0     = s.D2_full(f,aorg,used,scal,*drvs);
                    least_squares        &self   = *this;

                CYCLE:
                    ++cycle;
                    YACK_LSF_PRINTLN(clid << "---------------- cycle #" << cycle << " ----------------");
                    YACK_LSF_PRINTLN(clid << " f0 = " << f0 );

                    //----------------------------------------------------------
                    //
                    // compute curvature from sample
                    //
                    //----------------------------------------------------------
                    if( !computeCurv( lam[p10]) &&  !lam.increase(p10)) {
                        YACK_LSF_PRINTLN(clid << "<singular variables>");
                        return false;
                    }

                    //----------------------------------------------------------
                    //
                    // deduce step
                    //
                    //----------------------------------------------------------
                    computeStep();

                    //----------------------------------------------------------
                    //
                    // post-process
                    //
                    //----------------------------------------------------------
                    if(proc)
                    {
                        const bool success = (*proc)(f,aorg,s);
                        if(!success)
                        {
                            if(!lam.increase(p10))
                            {
                                YACK_LSF_PRINTLN(clid << "<cannot comply>");
                                return false;
                            }
                            f0 = s.D2_full(f,aorg,used,scal,*drvs);
                            goto CYCLE;
                        }
                    }

                    if(verbose)
                    {
                        ios::ocstream fp("linear.dat");
                        const size_t   np = 100;
                        for(size_t i=0;i<=np;++i)
                        {
                            const ORDINATE u = i/ORDINATE(np);
                            fp("%.15g %.15g\n", double(u), double(self(u)));
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // evaluate new position
                    //
                    //----------------------------------------------------------
                    ORDINATE       f1  = s.D2(f,aend);
                    const ORDINATE df  = f0-f1;
                    YACK_LSF_PRINTLN(clid << " f1 = " << f1 );
                    YACK_LSF_PRINTLN(clid << " df = " << df );


                    if(df>=0)
                    {
                        //------------------------------------------------------
                        //
                        // accepting
                        //
                        //------------------------------------------------------
                        YACK_LSF_PRINTLN(clid << "<accept>");
                        assert(f0>0);

                        //------------------------------------------------------
                        // check no overshoot when D2 decreases "a lot"
                        //------------------------------------------------------
                        const ORDINATE check_above = (1e-3) * f0;
                        if(df>=check_above)
                        {
                            YACK_LSF_PRINTLN(clid << "  <checking [df>=" << check_above << "]>");
                            check(f0,f1);

                            YACK_LSF_PRINTLN(clid << "  <checking/>");
                        }
                        else
                        {
                            YACK_LSF_PRINTLN(clid << "  <raw step [df<" << check_above << "]/>");
                        }
                        YACK_LSF_PRINTLN(clid << "<accept/>");

                        //------------------------------------------------------
                        // check variable convergence
                        //------------------------------------------------------
                        if(converged_variables(vars,used))
                            goto SUCCESS;

                        //------------------------------------------------------
                        // check D2 convergence ?
                        //------------------------------------------------------

                        //------------------------------------------------------
                        // ready for next FULL cycle
                        //------------------------------------------------------
                        lam.decrease(p10);
                        f0 = s.D2_full(f,aorg,used,scal,*drvs);
                        goto CYCLE;

                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // rejecting
                        //
                        //------------------------------------------------------
                        const ORDINATE limit = max_of(f1,f0) * numeric<ORDINATE>::ftol;
                        if( std::abs(df) <= limit )
                        {
                            YACK_LSF_PRINTLN(clid << "<accept below " << limit << ">");
                            goto SUCCESS;
                        }
                        else
                        {
                            YACK_LSF_PRINTLN(clid << "<reject above " << limit << ">");

                            //------------------------------------------------------
                            // ready for next SMALLER cycle
                            //------------------------------------------------------
                            if(!lam.increase(p10))
                            {
                                YACK_LSF_PRINTLN(clid << "<and spurious parameters!>");
                                return false;
                            }
                            goto CYCLE;
                        }

                    }

                    // never get here




                SUCCESS:
                    //----------------------------------------------------------
                    //
                    //
                    // starting point with full step
                    //
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // update variables
                    //----------------------------------------------------------
                    vars.mov(a0,aorg);

                    //----------------------------------------------------------
                    // compute errors
                    //----------------------------------------------------------
                    return err_(a0,used,scal,aerr);
                }


                //! compute errors with initialized workspace
                inline bool err_(writable<ORDINATE>       &a0,
                                 const readable<bool>     &used,
                                 const readable<ORDINATE> &scal,
                                 writable<ORDINATE>       &aerr)
                {
                    assert(NULL!=curr);
                    //----------------------------------------------------------
                    //
                    // initialize full step
                    //
                    //----------------------------------------------------------
                    YACK_LSF_PRINTLN(clid << "[computing errors]");
                    sample_type       &s = *curr;
                    const ORDINATE    f0 = s.D2_full(*hfcn,a0, used, scal, *drvs);
                    YACK_LSF_PRINTLN(clid << "|_D2     = " << f0);

                    //----------------------------------------------------------
                    //
                    // "exact" decomposition s.curv => this->curv
                    //
                    //----------------------------------------------------------
                    if(!computeCurv(0)) {
                        YACK_LSF_PRINTLN(clid << "<singular covariance>");
                        return false;
                    }

                    //----------------------------------------------------------
                    //
                    // use s.curv as inverse of this->curv
                    //
                    //----------------------------------------------------------
                    matrix<ORDINATE> &alpha = s.curv; assert( &alpha != &curv);
                    const variables  &vars  = **curr;
                    solv.inverse(curv,alpha);

                    for(const vnode *node=vars.head();node;node=node->next)
                    {
                        const variable &v = ***node;
                        const size_t    i = *v; if(used[i]) continue;
                        alpha[i][i] = 0;
                    }

                    //----------------------------------------------------------
                    //
                    // compute metrics
                    //
                    //----------------------------------------------------------
                    const size_t ndat = curr->dimension();
                    const size_t nvar = vars.size();
                    const size_t nact = vars.count(used);
                    YACK_LSF_PRINTLN(clid << "|_ndat = " << ndat);
                    YACK_LSF_PRINTLN(clid << "|_nvar = " << nvar);
                    YACK_LSF_PRINTLN(clid << "|_nact = " << nact);

                    if(ndat<nact)
                    {
                        YACK_LSF_PRINTLN(clid << "<not realistic>");
                        vars.ld(aerr,-1);
                    }
                    else
                    {
                        if(ndat>nact)
                        {
                            const size_t   ndof   = ndat-nact;
                            const ORDINATE value  = f0/ndof;
                            YACK_LSF_PRINTLN(clid << "|_ndof = " << ndof);
                            for(const vnode *node=vars.head();node;node=node->next)
                            {
                                const variable &v      = ***node;
                                const size_t    i      = *v;
                                const ORDINATE  stdvar = value * max_of<ORDINATE>(alpha[i][i],0);
                                aerr[i] = std::sqrt(stdvar/ndof);
                                if(verbose)
                                {
                                    vars.pad(std::cerr << "\t(*) "<< v.name,v.name);
                                    std::cerr << " = "   << std::setw(15) << a0[i];
                                    std::cerr << " +/- " << std::setw(15) << aerr[i];
                                    std::cerr << " [" << std::setw(6) << percent::of<ORDINATE,2>(a0[i],aerr[i]) << "%]";
                                    std::cerr << std::endl;
                                }
                            }
                        }
                        else
                        {
                            YACK_LSF_PRINTLN(clid << "<interpolation>");
                        }
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

                inline void check(const ORDINATE  f0,
                                  ORDINATE       &f1)
                {
                    static const ORDINATE utol = twice(numeric<ORDINATE>::sqrt_eps);
                    triplet<ORDINATE>     u    = { 0, -1,  1 };
                    triplet<ORDINATE>     f    = {f0, -1, f1 };
                    least_squares        &F    = *this;

                    if(locate::inside_for(F,u,f))
                    {
                        YACK_LSF_PRINTLN(clid << "  <located>");

                        bool _ = false;
                        YACK_LSF_PRINTLN(clid << "  |_" << u << " -> " << f);
                        while( optimize::tighten_for(F,u,f,_) > utol )
                        {
                            YACK_LSF_PRINTLN(clid << "  |_" << u << " -> " << f);
                        }

                        YACK_LSF_PRINTLN(clid << "  <located/>");
                        make_atry(u.b);
                        (**curr).mov(aend,atry);
                        f1 = f.b;
                    }
                    else
                    {
                        YACK_LSF_PRINTLN(clid << "  <global/>");
                    }

                }


                // test variables while moving aend to aorg
                inline bool converged_variables(const variables      &vars,
                                                const readable<bool> &used)
                {
                    static const ORDINATE xtol = numeric<ORDINATE>::sqrt_eps;

                    YACK_LSF_PRINTLN(clid << "<variables convergence testing>");

                    bool converged = true;
                    for(const vnode *node=vars.head();node;node=node->next)
                    {
                        const variable &v = ***node;
                        const size_t    i = *v; if(!used[i]) continue;
                        const ORDINATE  a_old = aorg[i];
                        const ORDINATE  a_new = aend[i];
                        const ORDINATE  delta = std::abs(a_old-a_new);
                        const ORDINATE  limit = xtol * max_of( std::abs(a_old), std::abs(a_new) );
                        const bool      is_ok = delta<=limit;
                        if(verbose)
                        {
                            vars.pad(std::cerr << ok(is_ok) << v.name,v.name);
                            std::cerr <<  " : " << std::setw(15) << a_old;
                            std::cerr << " -> " << std::setw(15) << a_new;
                            std::cerr << " [" << std::setw(15) << delta << "/" << limit << "]";
                            std::cerr << std::endl;
                        }
                        if(!is_ok) converged = false;
                        aorg[i] = a_new;
                    }
                    YACK_LSF_PRINTLN(clid << "<variables convergence =" << ok(converged) << ">");
                    return converged;
                }



            };

        }

    }

}

#endif
