#include "yack/gfx/filter.hpp"

namespace yack
{
    namespace graphic
    {
        filter:: weight:: weight() throw() :
        r(0,0),
        w(0)
        {
        }
        

        filter:: weight:: ~weight() throw() {}

        filter:: weight:: weight(const coord pos, const real_t val) throw() :
        r(pos),
        w(val)
        {
        }

        filter:: weight:: weight(const weight &other) throw() :
        r(other.r),
        w(other.w)
        {
        }

        std::ostream & operator<<(std::ostream &os, const filter::weight &w)
        {
            os << w.w << "@" << w.r;
            return os;
        }

    }

}

namespace yack
{
    namespace graphic
    {
        filter:: ~filter() throw()
        {
        }

        const string & filter:: key() const throw() { return name; }

        std::ostream & operator<<(std::ostream &os, const filter &F)
        {
            os << F.name << '=' << F.data;
            return os;
        }


    }

}
