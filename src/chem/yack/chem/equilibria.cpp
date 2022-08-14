
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
            absorb(*eq);
            return *eq;
        }
        
        equilibria:: equilibria(const equilibria &other) :
        gathering()
        {
            for(const enode *node=other.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
               (void) use( & coerce(eq) );
            }
        }

        std::ostream & operator<<(std::ostream &os, const equilibria &eqs)
        {
            os << '{' << std::endl;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eqs.pad(os << "  <" << eq.name << ">",eq);
                
                os << std::endl;
            }
            
            os << '}';
            return os;
        }
        
        
    }
    
}

