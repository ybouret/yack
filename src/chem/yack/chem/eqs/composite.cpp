

#include "yack/chem/eqs/composite.hpp"

namespace yack
{
    namespace chemical
    {

        composite:: coeff:: coeff() throw() : lhs(0), rhs(0) {}


        void composite:: coeff:: normalize() throw()
        {
            if(rhs<lhs) cswap(lhs,rhs);
        }

        size_t composite:: collect(coeffs           &cof,
                                   const components &lhs,
                                   const components &rhs)
        {

            cof.free();

            for(const cnode *lnode=lhs.head();lnode;lnode=lnode->next)
            {
                const component &lcomp  = ***lnode;
                const size_t     lindx  =  **lcomp;
                const int        lcoef  = lcomp.nu;

                for(const cnode *rnode=rhs.head();rnode;rnode=rnode->next)
                {
                    const component &rcomp  = ***rnode;
                    const size_t     rindx  =  **rcomp;

                    if(rindx!=lindx) continue;

                    std::cerr << "\t\tsharing " << (*rcomp).name << std::endl;

                    bool  found = false;
                    coeff guess(lcoef,rcomp.nu);
                    guess.normalize();

                    for(size_t i=cof.size();i>0;--i)
                    {
                        if(guess == cof[i]) { found=true; break; }
                    }

                    if(!found)
                    {
                        cof << guess;
                    }

                }

            }


            return cof.size();
        }


    }

}
