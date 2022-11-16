
#include "yack/chem/reactor.hpp"
#include "yack/comparison.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {
        reactor:: tumbler:: tumbler(const double _f, const species & _s) throw() :
        f(_f),
        s(_s)
        {
        }

        reactor:: tumbler:: tumbler(const tumbler &_) throw() : f(_.f), s(_.s) {}

        reactor:: tumbler:: ~tumbler() throw() {}

        std::ostream & operator<<(std::ostream &os, const reactor:: tumbler &a)
        {
            os << '\t' <<  '(' << '[' << a.s.name << ']' <<  '@' << a.f << ')';
            return os;
        }

        int reactor:: tumbler:: compare(const tumbler &lhs, const tumbler &rhs) throw()
        {
            return comparison::increasing(lhs.f,rhs.f);
        }
    }

}

