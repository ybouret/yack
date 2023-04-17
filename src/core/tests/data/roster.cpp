#include "yack/ordered/roster.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace
{
    struct comp2sign
    {
        template <typename T>
        inline sign_type operator()(const T & lhs, const T & rhs) noexcept
        {
            return __sign::of(lhs,rhs);
        }
    };
}
YACK_UTEST(data_roster)
{
    {
        roster<int,comp2sign> rs;

        rs.push(1);
        rs.push(-2);
        std::cerr << rs << std::endl;

        rs.free();
        rs.push(3);
        rs.push(4);
        std::cerr << rs << std::endl;
    }

    {
        roster<string,comp2sign> rs;
        rs.push("world");
        rs.push("hello");
        std::cerr << rs << std::endl;
    }


}
YACK_UDONE()


