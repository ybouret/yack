

#include "yack/chem/reactor/restriction.hpp"
#include "yack/math/iota.hpp"
#include "yack/type/utils.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        restriction::  restriction() throw() : object(), counted(), actors(), q2(0) {}
        restriction:: ~restriction() throw() {}

        bool restriction:: compile() throw()
        {
            {
                unsigned &sum = coerce(q2);
                sum = 0;
                for(const actor *a=(*this)->head;a;a=a->next)
                    sum += squared(a->nu);
            }
            return q2>0;

        }

        double restriction:: apply(writable<double>       &target,
                                   const readable<double> &source,
                                   raddops                &xadd) const
        {
            assert(q2>0);
            iota::load(target,source);
            xadd.free();
            for(const actor *a=(*this)->head;a;a=a->next)
            {
                xadd.push( a->nu * source[ ***a ] );
            }
            const double value = xadd.get();
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



    }

}

