#include "yack/ordered/roster.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    struct icompare
    {
        inline int operator()(const int lhs, const int rhs) noexcept
        {
            return lhs-rhs;
        }
    };
}
YACK_UTEST(data_roster)
{
    roster<int,icompare> rs;

    rs.push(1);
}
YACK_UDONE()


