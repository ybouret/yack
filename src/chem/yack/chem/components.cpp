
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
        d_nu(0),
        cdb(),
        xlm()
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
        
        size_t components:: size() const throw()
        {
            return (*cdb.tree).size;
        }

        
        const xlimits & components:: genuine_limits(const readable<double> &C, const size_t w) const throw()
        {
            return * new ( *xlm ) xlimits(reac.genuine_limit(C),prod.genuine_limit(C),w);
        }
        
        void components:: operator()(const species &sp,
                                     const int      nu)
        {

            // create new component
            const component::pointer cptr = new component(sp,nu);
            if(!cdb.insert(cptr)) throw imported::exception(clid,"muliple '%s'",sp.name());

            // update actors
            try {
                switch( __sign::of(nu) )
                {
                    case __zero__:
                        throw imported::exception(clid,"invalid 0 * '%s'",sp.name());

                    case positive:
                        coerce(prod)(sp,nu);
                        break;

                    case negative:
                        coerce(reac)(sp,-nu);
                        break;
                }
            }
            catch(...)
            {
                (void) cdb.remove(sp.name);
                throw;
            }
            
            // update
            ++(coerce(sp.rank));
            coerce(d_nu) = int(prod.molecularity) - int(reac.molecularity);
        }
        
        double components:: mass_action(const double            K,
                                        const readable<double> &C,
                                        rmulops                &ops) const
        {
            // reactant side
            ops.free();
            ops.push(K);
            const double rma = reac.mass_action(C,ops);

            // product side
            ops.free();
            const double pma = prod.mass_action(C,ops);

            // difference
            return rma - pma;
        }

        double components:: mass_action(const double            K,
                                        const readable<double> &C,
                                        const double            xi,
                                        rmulops                &ops) const
        {
            // reactant side
            ops.free();
            ops.push(K);
            const double rma = reac.mass_action(C,-xi,ops);

            // product side
            ops.free();
            const double pma = prod.mass_action(C,xi,ops);

            // difference
            return rma - pma;
        }

    }


}

#include "yack/arith/gcd.h"

namespace yack
{
    namespace chemical
    {

        bool components:: neutral() const throw()
        {
            return reac.algebraic_Z == prod.algebraic_Z;
        }

        bool components:: minimal() const throw()
        {
            if(size()<=1)
            {
                return true;
            }
            else
            {
                assert(size()>=2);
                const cnode   *node = head();              assert(node);
                uint64_t       gcd  = abs( (**node)->nu ); assert(gcd>0);
                for(node=node->next;node;node=node->next)
                {
                    gcd = yack_gcd64(gcd, abs( (**node)->nu ) );
                }
                return 1 == gcd;
            }
        }

        components::state components:: state_at(const readable<double> &C) const throw()
        {
            return (reac.are_blocked_by(C) && prod.are_blocked_by(C)) ? are_blocked : are_running;
        }


    }
    
}
