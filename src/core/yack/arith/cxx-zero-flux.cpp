#include "yack/arith/cxx-zero-flux.hpp"

namespace yack
{

    cxx_zero_flux:: cxx_zero_flux(unit_t n) throw() :
    size(n),
    szsz(n+n),
    get_(size<=1? &cxx_zero_flux::get1 : &cxx_zero_flux::getn )
    {
        assert(size>0);
    }

    unit_t cxx_zero_flux::get1(const unit_t) const throw()
    {
        return 1;
    }

    unit_t cxx_zero_flux::getn(const unit_t indx) const throw()
    {
        if(indx<=0)
        {
            return (*this)(2-indx);
        }
        else
        {
            if(indx>size)
            {
                return (*this)( szsz - indx );
            }
            else
            {
                return indx;
            }
        }
    }

    cxx_zero_flux:: cxx_zero_flux(const cxx_zero_flux &other) throw() :
    size(other.size),
    szsz(other.szsz),
    get_(other.get_)
    {
        assert(get_);
    }

    cxx_zero_flux:: ~cxx_zero_flux() throw()
    {
    }

    unit_t cxx_zero_flux:: operator()(const unit_t indx) const throw()
    {
        assert(get_);
        return (this->*get_)(indx);
    }


}
