

#include "yack/chem/eqs/composite.hpp"

namespace yack
{
    namespace chemical
    {

        composite:: coeff:: coeff() throw() : lhs(0), rhs(0) {}


        size_t composite:: collect(coeffs           &cof,
                                   const components &lhs,
                                   const components &rhs)
        {

            cof.free();

            


            return cof.size();
        }


    }

}
