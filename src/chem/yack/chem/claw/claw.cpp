
#include "yack/chem/claw/claw.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    using namespace math;
    
    namespace chemical
    {
        
        conservation_law:: ~conservation_law() throw() {}
        conservation_law::  conservation_law() throw() :
        object(),
        actors(),
        next(0),
        prev(0),
        nrm2(0),
        indx(0)
        {}
        
        
        std::ostream & operator<<(std::ostream &os, const conservation_law &self)
        {
            const actors &me = self;
            os << "d(" << me << ")";
            return os;
        }
        
        void   conservation_law:: finalize(const size_t i) throw()
        {
            coerce(nrm2) = 0;
            coerce(indx) = i; assert(indx>0);
            for(const actor *a=crew.head;a;a=a->next)
            {
                coerce(nrm2) += squared(a->nu);
            }
            
        }
        
        double conservation_law:: evaluate(const readable<double> &C, math::adder<double> &xadd) const
        {
            xadd.ldz();
            for(const actor *a=crew.head;a;a=a->next)
            {
                xadd.push( C[***a] * a->nu );
            }
            return xadd.get();
        }
        
        bool conservation_law:: regulate(writable<double>       &target,
                                         const readable<double> &source,
                                         raddops                &xadd) const
        {
            assert(nrm2>0);
            const double xs = evaluate(source,xadd);
            if(xs<0)
            {
                const double factor = -xs;
                xadd.ldz();
                iota::load(target,source);
                for(const actor *a=crew.head;a;a=a->next)
                {
                    const double  d = (factor*a->nu)/nrm2;
                    target[***a] += d;
                    xadd.push(d);
                }
                assert(xadd.size()==crew.size);
                return true;
            }
            else
            {
                return false;
            }
        }

        void conservation_law:: injected(writable<double>       &inset,
                                         const readable<double> &source,
                                         const readable<double> &target) const throw()
        {
            for(const actor *a=crew.head;a;a=a->next)
            {
                const size_t j = ***a;
                const double d = target[j] - source[j]; assert(d>=0);
                inset[j] += d;
            }
        }
        
    }
    
}



