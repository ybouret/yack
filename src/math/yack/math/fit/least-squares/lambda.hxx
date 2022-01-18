
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
                static const real_t ten(10);
                field_type &self = *this;
                assert(fabs(self[lower])<=0);
                for(unit_t p=lower+1;p<=upper;++p)
                {
                    self[p] = pow(ten,static_cast<real_t>(p));
                }
            }



        }

    }

}
