#include "yack/gfx/zero-flux.hpp"

namespace yack
{
    namespace graphic
    {
        zero_flux:: zero_flux(unit_t n) throw() :
        size(n),
        szsz(n+n)
        {
        }

        zero_flux:: ~zero_flux() throw()
        {
        }

        unit_t zero_flux:: operator()(unit_t indx) const throw()
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

}
