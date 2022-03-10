
#include "yack/chem/limits.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    namespace chemical
    {
        static inline
        limited_type get_limited_type(const limit *r, const limit *p) throw()
        {
            if(r)
            {
                // limited by reac
                if(p)
                {
                    return limited_by_both;
                }
                else
                {
                    return limited_by_reac;
                }
            }
            else
            {
                // not limited by reac
                if(p)
                {
                    return limited_by_prod;
                }
                else
                {
                    return limited_by_none;
                }
            }
        }

        limits:: limits(const limit *r, const limit *p) throw() :
        reac(r),
        prod(p),
        type( get_limited_type(r,p) )
        {

        }

        std::ostream & operator<<(std::ostream &os, const limits &lm)
        {
            switch(lm.type)
            {
                case limited_by_none:
                    os << "unlimited";
                    break;

                case limited_by_reac:
                    os << "limited by reactant [" << (***(lm.reac)).name << "]: xi <= " << lm.reac_extent();
                    break;

                case limited_by_prod:
                    os << "limited by product  [" << (***(lm.prod)).name << "]: xi >= " << lm.prod_extent();
                    break;

                case limited_by_both:
                    os << "limited by reactant [" << (***(lm.reac)).name << "] and product [" << (***(lm.prod)).name << "]";
                    os << ": xi in [" <<  lm.prod_extent() << ":" << lm.reac_extent() << "]";
                    break;
            }
            return os;
        }


        double limits:: reac_extent() const throw()
        {
            assert(reac);
            assert(reac->xi>=0);
            return reac->xi;
        }

        double limits:: prod_extent() const throw()
        {
            assert(prod);
            assert(prod->xi>=0);
            const double xi = prod->xi;
            return (xi>0?-xi:0);
        }

        void limits:: clamp(double &xi) const throw()
        {
            switch(type)
            {
                case limited_by_none: break;
                case limited_by_reac: xi = yack::min_of(xi,reac->xi); break;
                case limited_by_prod: xi = yack::max_of(-prod->xi,xi); break;
                case limited_by_both: xi = yack::clamp(-prod->xi,xi,reac->xi); break;
            }

        }


    }

}

