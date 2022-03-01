#include "yack/chem/actor.hpp"
#include "yack/arith/ipower.hpp"

namespace yack
{
    namespace chemical
    {
        actor:: ~actor() throw()
        {
        }

        actor:: actor(const species  &sp,
                      const unsigned  cf) throw() :
        object(),
        next(0),
        prev(0),
        coef(cf),
        cder(coef-1),
        host(sp)
        {
            assert(coef>0);
        }

        const species & actor:: operator*() const throw() { return host; }

        double actor:: limiting(const readable<double> &C) const throw()
        {
            return host[C]/coef;
        }

        double actor:: mass_action(const readable<double> &C) const throw()
        {
            return ipower(host[C],coef);
        }

        double actor:: drvs_action(const readable<double> &C) const throw()
        {
            return coef * ipower(host[C],cder);
        }

        void actor:: display(std::ostream &os, const bool first) const
        {
            if(!first) os << '+';
            if(coef>1) os << coef;
            os << '[' << host.name << ']';
        }

        
    }

}

