#include "yack/arith/raw-zero-flux.hpp"

namespace yack
{

    raw_zero_flux:: raw_zero_flux(unit_t n) throw() :
    size(n),
    szsz(n+n),
    get_(size<=1? & raw_zero_flux::get1 : & raw_zero_flux::getn )
    {
    }

    raw_zero_flux:: raw_zero_flux(const raw_zero_flux &other) throw() :
    size(other.size),
    szsz(other.szsz),
    get_(other.get_)
    {

    }

    raw_zero_flux:: ~raw_zero_flux() throw()
    {
    }

    unit_t raw_zero_flux:: get1(unit_t) const throw()
    {
        return 0;
    }


    unit_t raw_zero_flux:: getn(unit_t indx) const throw()
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
    
    unit_t raw_zero_flux:: operator()(const unit_t indx) const throw()
    {
        assert(get_);
        return (this->*get_)(indx);
    }


}
