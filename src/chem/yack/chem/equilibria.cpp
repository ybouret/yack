
#include "yack/chem/equilibria.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {
        equilibria:: ~equilibria() throw() {}
        
        equilibria:: equilibria() throw() : gathering()
        {
        }
        
        size_t equilibria:: size() const throw()
        {
            return (*edb.tree).size;
        }
        
        const enode *equilibria:: head() const throw()
        {
            return (*edb.tree).head;
        }

        
        const char equilibria::clid[] = "chemical::equilibria";
        
        equilibrium &equilibria:: use(equilibrium *eq)
        {
            assert(NULL!=eq);
            const equilibrium::pointer p(eq);
            if(**p!=size()+1)  throw imported::exception(clid,"bad index for <%s>",eq->name());
            if(!edb.insert(p)) throw imported::exception(clid,"multiple <%s>", eq->name());
            return *eq;
        }
        
    }
    
}

