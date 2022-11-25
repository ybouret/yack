

#include "yack/chem/reactor/restriction.hpp"
#include "yack/math/iota.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        restriction::  restriction() throw() : object(), counted(), actors(), q2(0), ii(0) {}
        restriction:: ~restriction() throw() {}

        bool restriction:: compile(const size_t indx) throw()
        {
            assert(indx>0);
            {
                unsigned &sum = coerce(q2);
                sum = 0;
                for(const actor *a=(*this)->head;a;a=a->next)
                    sum += squared(a->nu);
            }
            coerce(ii) = indx;
            return q2>0;

        }


        double restriction:: compute(const readable<double> &source,
                                     raddops                &xadd) const throw()
        {
            xadd.free();
            for(const actor *a=(*this)->head;a;a=a->next)
            {
                xadd.push( a->nu * source[ ***a ] );
            }
            return xadd.get();
        }


        double restriction:: apply(writable<double>       &target,
                                   const readable<double> &source,
                                   raddops                &xadd) const throw()
        {
            assert(q2>0);
            iota::load(target,source);
            const double value = compute(source,xadd);
            if(value>=0)
            {
                return 0;
            }
            else
            {
                const double Xs = -value; assert(Xs>0);
                for(const actor *a=(*this)->head;a;a=a->next)
                {
                    target[***a] += (a->nu * Xs) / q2;
                }
                return Xs;
            }
        }

        size_t restriction:: operator*() const throw() { return ii; }


        bool restriction:: overlaps(const restriction &other) const throw()
        {
            for(const actor *lhs=(*this)->head;lhs;lhs=lhs->next)
            {
                const species &l = **lhs;
                for(const actor *rhs=other->head;rhs;rhs=rhs->next)
                {
                    const species &r = **rhs;
                    if( &l == &r ) return true;
                }
            }
            return false;
        }


    }

}

