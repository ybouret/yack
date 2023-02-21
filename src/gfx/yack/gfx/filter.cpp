#include "yack/gfx/filter.hpp"

namespace yack
{
    namespace graphic
    {
        filter:: weight:: weight() noexcept :
        r(0,0),
        w(0)
        {
        }
        

        filter:: weight:: ~weight() noexcept {}

        filter:: weight:: weight(const coord pos, const real_t val) noexcept :
        r(pos),
        w(val)
        {
        }

        filter:: weight:: weight(const weight &other) noexcept :
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
        filter:: ~filter() noexcept
        {
        }

        const string & filter:: key() const noexcept { return name; }

        std::ostream & operator<<(std::ostream &os, const filter &F)
        {
            os << F.name << '=' << F.data;
            return os;
        }


    }

}
