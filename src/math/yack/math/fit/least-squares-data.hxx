
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
            large_object(),
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
                    const real_t da = fabs( step[i] );
                    const real_t aa = fabs( atmp[i] );
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
                                                          const real_t           g1,
                                                          const real_t           sigma) const
            {
                static const real_t one(1);

                assert(g1<=g0);
                assert(sigma>=0);

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

                // choose the minimum
                if(verbose)
                {
                    std::cerr << "[LS] minimum in u=" << U << ", g=" << G << std::endl;
                    std::cerr << "[LS] estimated [" << G_opt << " @" << U_opt << "]" << std::endl;
                }

                return G_opt;

            }


        }

    }

}

