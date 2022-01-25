
namespace yack
{
    namespace math
    {
        namespace fit
        {

            template <>
            least_squares_data<real_t>:: ~least_squares_data() throw()
            {
            }

            template <>
            least_squares_data<real_t>:: least_squares_data() :
            least_squares_info(),
            tableaux(2,0),
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

            template <>
            bool least_squares_data<real_t>::converged(const readable<real_t> &atmp) const throw()
            {
                assert( atmp.size() == step.size() );

                for(size_t i=atmp.size();i>0;--i)
                {
                    const real_t da = std::abs( step[i] );
                    const real_t aa = std::abs( atmp[i] );
                    if( da > ftol * aa ) return false;
                }

                return true;
            }

            template <>
            void least_squares_data<real_t>:: initialize(const size_t npar)
            {
                make(npar);
                curv.make(npar,npar);
                shrinking = false;
                if(algo.is_empty()||algo->dims<npar)
                {
                    algo = new LU(npar);
                }
            }

            template <>
            bool  least_squares_data<real_t>::shrink(unit_t &p) throw()
            {
                shrinking = true;
                return ++p <= pmax;
            }

            template <>
            bool least_squares_data<real_t>:: compute_curvature(unit_t                 &p,
                                                                const matrix<real_t>   &covm,
                                                                const variables        &vars,
                                                                const readable<bool>   &used)
            {
                assert(p>=this->lam.lower);
                assert(p<=this->lam.upper);
                assert(used.size()==covm.rows);
                assert(used.size()==covm.cols);
                static const real_t one(1);
                const        size_t npar = used.size();

            TRY:
                const real_t  dfac = one + lam[p];
                curv.assign(covm);
                for(size_t i=npar;i>0;--i)
                {
                    writable<real_t> &curv_i = curv[i];
                    if(!used[i]||!vars.handles(i))
                    {
                        curv_i[i] = one;
                    }
                    else
                    {
                        curv_i[i] *= dfac;
                    }
                }

                const bool regular = algo->build(curv);
                if(!regular)
                {
                    if(shrink(p))
                    {
                        assert(p>pmax);
                        return false; // singular
                    }
                    goto TRY;
                }

                // ok
                return true;
            }


            template <>
            void  least_squares_data<real_t>:: compute_errors(writable<real_t>     &aerr,
                                                              matrix<real_t>       &covm,
                                                              const variables      &vars,
                                                              const readable<bool> &used,
                                                              const size_t          ndat)
            {
                assert(aerr.size()==used.size());
                assert(aerr.size()==covm.rows);
                assert(aerr.size()==covm.cols);
                assert(D2>=0);
                static const real_t one(1);
                const        size_t npar = used.size();
                size_t              nfit = npar;

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------

                initialize(npar);
                aerr.ld(-1);
                curv.assign(covm);

                //--------------------------------------------------------------
                //
                // regularize
                //
                //--------------------------------------------------------------
                for(size_t i=npar;i>0;--i)
                {
                    writable<real_t> &curv_i = curv[i];
                    if(!used[i]||!vars.handles(i))
                    {
                        curv_i[i] = one;
                        aerr[i]   = 0;
                        --nfit;
                    }
                }

                if(verbose)
                {
                    std::cerr << clid << " #params = " << npar << std::endl;
                    std::cerr << clid << " #active = " << nfit << std::endl;
                    std::cerr << clid << " #data   = " << ndat << std::endl;
                }

                //--------------------------------------------------------------
                //
                // compute inverse of covariance matrix
                //
                //--------------------------------------------------------------
                if(!algo->build(curv))
                {
                    throw exception("%s: singular covariance", clid);
                }
                algo->inv(covm,curv);

                //--------------------------------------------------------------
                //
                // compute total variance and degrees of freedom
                //
                //--------------------------------------------------------------
                real_t sig2 = 0;
                size_t ndof = ndat;
                switch( __sign::of(ndat,nfit))
                {
                    case negative: assert(ndat<nfit);
                        throw exception("%s: too many active #active=%u for #data=%u",clid, unsigned(nfit), unsigned(ndat));
                        break;

                    case __zero__:
                        YACK_LSF_PRINTLN(clid << " interpolation"); // keep error to zero
                        ndof=1;
                        break;

                    case positive: assert(ndat>nfit);
                        sig2  = D2;
                        ndof -= nfit;
                        break;
                }

                sig2 /= ndof;
                YACK_LSF_PRINTLN(clid << " D2   = " << D2);
                YACK_LSF_PRINTLN(clid << " ndof = " << ndof);
                YACK_LSF_PRINTLN(clid << " sig2 = " << sig2);


                for(size_t i=npar;i>0;--i)
                {
                    if(!used[i]||!vars.handles(i))
                    {
                        continue;
                    }

                    const real_t variance = sig2 * std::abs(covm[i][i]);
                    aerr[i] = sqrt(variance/ndat);
                }

                if(verbose) vars.display(std::cerr,aerr,"\terr.");

            }
            

            
        }

    }

}


namespace yack
{
    namespace math
    {
        namespace fit
        {

            static inline bool acceptable(const triplet<real_t> &G) throw()
            {
                return (G.b<=G.a && G.b<=G.c);
            }

            template <>
            real_t  least_squares_data<real_t>:: optimize(real_function<real_t> &g,
                                                          const real_t           g0,
                                                          const real_t           g1) const
            {
                static const real_t one(1);

                assert(g1<=g0);
                
                //______________________________________________________________
                //
                // take at least another sample
                //______________________________________________________________

                triplet<real_t> U = {0, 1, 2     };
                triplet<real_t> G = {g0,g1,g(U.c)};
                assert(G.b<=G.a);

                while( !acceptable(G) )
                {
                    U.a = U.b; U.b = U.c;
                    G.a = G.b; G.b = G.c;
                    G.c = g( U.c += one);
                }

                //______________________________________________________________
                //
                // now we are around a minimum
                //______________________________________________________________
                const real_t U_opt = minimize::parabolic_guess(U,G);
                const real_t G_opt = g(U_opt);

                YACK_LSF_PRINTLN(clid << " minimum in u=" << U << ", g=" << G);
                YACK_LSF_PRINTLN(clid << " estimated [" << G_opt << " @ " << U_opt << "]");

                return G_opt;

            }


        }

    }

}

