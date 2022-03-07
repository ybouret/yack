#include "yack/arith/cxx-zero-flux.hpp"

namespace yack
{

    cxx_zero_flux:: cxx_zero_flux(unit_t n) throw() :
    size(n),
    szsz(n+n)
    {
    }

    cxx_zero_flux:: cxx_zero_flux(const cxx_zero_flux &other) throw() :
    size(other.size),
    szsz(other.szsz)
    {

    }

    cxx_zero_flux:: ~cxx_zero_flux() throw()
    {
    }

    unit_t cxx_zero_flux:: operator()(unit_t indx) const throw()
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
