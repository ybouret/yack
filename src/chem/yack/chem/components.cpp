
#include "yack/chem/components.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {
        const char components:: clid[] = "chemical::components";
        
        components:: ~components() throw()
        {
        }
        
        
        components:: components() throw() :
        reac(),
        prod(),
        cdb()
        {
            
        }
        
        std::ostream & operator<<(std::ostream &os, const components &comp)
        {
            os << comp.reac << " <=> " << comp.prod;
            return os;
        }
        
        const cnode * components:: head() const throw()
        {
            return (*cdb.tree).head;
        }
        
        const xlimits & components:: genuine_limits(const readable<double> &C, const size_t w) const throw()
        {
            return * new( xlm.get_entry() ) xlimits(reac.genuine_limit(C),prod.genuine_limit(C),w);
        }
        
        void components:: operator()(const species &sp,
                                     const int      nu)
        {
            const component::pointer cptr = new component(sp,nu);
            if(!cdb.insert(cptr)) throw imported::exception(clid,"muliple '%s'",sp.name());
            
            // select actors
            switch( __sign::of(nu) )
            {
                case __zero__:
                    cdb.remove(sp.name);
                    throw imported::exception(clid,"invalid 0 * '%s'",sp.name());
                    
                case positive:
                    coerce(prod)(sp,nu);
                    break;
                    
                case negative:
                    coerce(reac)(sp,-nu);
                    break;
            }
            
            
            // update
            ++(coerce(sp.rank));
        }
    }
    
}
