
#include "yack/chem/eq/limits.hpp"

namespace yack
{

    namespace chemical
    {

        limits::  limits(const limiting *r, const limiting *p) throw() :
        reac(r), prod(p), type( make() )
        {
        }

        limiting::status limits::make() const throw()
        {
            if(reac)
            {
                if(prod)
                {
                    return limiting::by_both;
                }
                else
                {
                    return limiting::by_reac;
                }
            }
            else
            {
                if(prod)
                {
                    return limiting::by_prod;
                }
                else
                {
                    return limiting::by_none;
                }
            }
        }


        std::ostream & operator<<(std::ostream &os, const limits &l)
        {
            switch(l.type)
            {
                case limiting::by_none: os << "not limited"; break;
                case limiting::by_reac: os << "limited by reactant " << (**(l.reac->pa)).name << ", xi<=" << l.reac->xi;  break;
                case limiting::by_prod: os << "limited by product  " << (**(l.prod->pa)).name << ", xi>=-" << l.prod->xi; break;
                case limiting::by_both:
                    os << "limited by reactant " << (**(l.reac->pa)).name << " and product " <<(**(l.prod->pa)).name;
                    os << ", xi in [-" << l.prod->xi << ":" << l.reac->xi << "]";
            }

            return os;
        }

    }

}

