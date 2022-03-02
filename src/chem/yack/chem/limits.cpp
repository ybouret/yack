
#include "yack/chem/limits.hpp"

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
                    os << "limited by reactant [" << (***(lm.reac)).name << "]: xi <= " << lm.reac->xi;
                    break;

                case limited_by_prod:
                    os << "limited by product  [" << (***(lm.prod)).name << "]: xi >= " << -lm.prod->xi;
                    break;

                case limited_by_both:
                    os << "limited by reactant [" << (***(lm.reac)).name << "] and product [" << (***(lm.prod)).name << "]";
                    os << ": xi in [" << -lm.prod->xi << ":" << lm.reac->xi << "]";
                    break;
            }
            return os;
        }

    }

}

