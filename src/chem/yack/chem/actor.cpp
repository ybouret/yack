#include "yack/chem/actor.hpp"
#include "yack/arith/ipower.hpp"
#include "yack/type/utils.hpp"

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
        csqr(coef*coef),
        host(sp)
        {
            assert(coef>0);
        }

        const species & actor:: operator*() const throw() { return host; }

        double actor:: limiting_extent(const readable<double> &C) const throw()
        {
            assert(C[*host]>=0);
            return C[*host]/coef;
        }

        double actor:: mass_action(const readable<double> &C) const throw()
        {
            return ipower(C[*host],coef);
        }

        double actor:: mass_action(const readable<double> &C, const double xi) const throw()
        {
            const double Ci = C[*host]; assert(Ci>=0);
            return ipower( max_of(0.0,Ci+coef*xi), coef );
        }

        void actor:: move(writable<double> &C, const double xi) const throw()
        {
            double &Ci = C[*host]; assert(Ci>=0);
            Ci = max_of(0.0,Ci+coef*xi);
        }


        double actor:: drvs_action(const readable<double> &C) const throw()
        {
            return coef * ipower(C[*host],cder);
        }

        void actor:: display(std::ostream &os, const bool first) const
        {
            if(!first) os << '+';
            if(coef>1) os << coef;
            os << '[' << host.name << ']';
        }

        double actor:: slope(const readable<double> &C, const double xi) const throw()
        {
            const double Ci =  C[*host]; assert(Ci>=0);
            return csqr * ipower( max_of(0.0,Ci+coef*xi), cder );
        }

        
    }

}

