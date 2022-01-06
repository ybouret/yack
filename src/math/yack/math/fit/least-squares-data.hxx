
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

