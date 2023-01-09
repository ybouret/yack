#include "yack/chem/indexed.hpp"

namespace yack
{
    namespace chemical
    {
        indexed:: ~indexed() throw()
        {
            coerce(indx_) = 0;
        }

        indexed:: indexed(const size_t i) throw() :
        indx_(i)
        {
            assert(indx_>0);
        }

        indexed:: indexed(const indexed &_) throw() :
        indx_(_.indx_)
        {

        }


        size_t indexed:: operator*() const throw() { assert(indx_>0); return indx_; }

    }

}

