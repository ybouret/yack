#include "yack/arith/raw-zero-flux.hpp"

namespace yack
{

    raw_zero_flux:: raw_zero_flux(unit_t n) throw() :
    size(n),
    szsz(n+n)
    {
    }

    raw_zero_flux:: raw_zero_flux(const raw_zero_flux &other) throw() :
    size(other.size),
    szsz(other.szsz)
    {

    }

    raw_zero_flux:: ~raw_zero_flux() throw()
    {
    }

    unit_t raw_zero_flux:: operator()(unit_t indx) const throw()
    {
        if(indx<0)
        {
            return (*this)(-indx);
        }
        else
        {
            if(indx>=size)
            {
                return (*this)( szsz - (++indx) );
            }
            else
            {
                return indx;
            }
        }

    }


}
