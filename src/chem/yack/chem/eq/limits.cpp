
#include "yack/chem/eq/limits.hpp"

namespace yack
{

    namespace chemical
    {

        limits::  limits(const limiting *r, const limiting *p) throw() :
        reac(r), prod(p), type( make() )
        {
        }

        limitation limits::make() const throw()
        {
            if(reac)
            {
                if(prod)
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
                if(prod)
                {
                    return limited_by_prod;
                }
                else
                {
                    return limited_by_none;
                }
            }
        }


        std::ostream & operator<<(std::ostream &os, const limits &l)
        {
            switch(l.type)
            {
                case limited_by_none: os << "not limited"; break;
                case limited_by_reac: os << "limited by reactant " << (**(l.reac->pa)).name << ", xi<=" << l.reac->xi;  break;
                case limited_by_prod: os << "limited by product  " << (**(l.prod->pa)).name << ", xi>=-" << l.prod->xi; break;
                case limited_by_both:
                    os << "limited by reactant " << (**(l.reac->pa)).name << " and product " <<(**(l.prod->pa)).name;
                    os << ", xi in [-" << l.prod->xi << ":" << l.reac->xi << "]";
            }

            return os;
        }

    }

}

