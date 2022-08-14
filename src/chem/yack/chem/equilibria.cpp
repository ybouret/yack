
#include "yack/chem/equilibria.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>
#include "yack/sequence/list.hpp"

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
            list<string> Ks(eqs.size(),as_capacity);
            size_t klen = 0;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const string ks = vformat("%.15g",eq.K(equilibrium::display_time));
                Ks << ks;
                klen = max_of(klen,ks.size());
            }
            list<string>::iterator it = Ks.begin();
            for(const enode *node=eqs.head();node;node=node->next,++it)
            {
                const equilibrium &eq = ***node;
                eqs.pad(os << "  <" << eq.name << ">",eq);
                const string &ks = *it;
                os << " (" << ks;
                for(size_t i=ks.size();i<klen;++i) os << ' ';
                os << ")";
                os << ' ' << static_cast<const components &>(eq);
                os << std::endl;
            }
            
            os << '}';
            return os;
        }
        
        
    }
    
}

