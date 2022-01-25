
namespace yack
{
    namespace math
    {
        namespace fit
        {

            template <>
            unit_t lambda<real_t>:: pmin() throw()
            {
                return floor( log10( numeric<real_t>::epsilon) );
            }

            template <>
            unit_t lambda<real_t>:: pmax() throw()
            {
                return numeric<real_t>::max_10_exp;
            }

            template <> lambda<real_t>:: ~lambda() throw()
            {
            }

            template <> lambda<real_t>:: lambda() :
            field_type("lambda", layout1D(pmin(),pmax())) 
            {

                field_type &self = *this;
                assert(std::abs(self[lower])<=0);
                self[0] = 1;
                
                for(unit_t p=lower+1;p<0;++p)
                {
                    static const real_t fac(0.1);
                    self[p] = ipower<real_t,unit_t>(fac,-p);
                }

                for(unit_t p=1;p<=upper;++p)
                {
                    static const real_t fac(10);
                    self[p] = ipower<real_t,unit_t>(fac,p);
                }


            }



        }

    }

}
