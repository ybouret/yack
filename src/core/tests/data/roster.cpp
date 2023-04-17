#include "yack/ordered/roster.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/randomized/shuffle.hpp"

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


    template <typename T>
    static inline void perform(randomized::bits &ran)
    {


        roster<T,comp2sign> rs;
        for(size_t cycle=1;cycle<=4;++cycle)
        {
            rs.free();
            for(size_t i=ran.leq(10)+1;i>0;--i)
            {
                const T tmp = bring::get<T>(ran);
                for(size_t j=ran.leq(3)+1;j>0;--j)
                {
                    rs.insert(tmp);
                }
            }
            std::cerr << rs << std::endl;
        }

        rs.free();
        for(size_t cycle=1;cycle<=4;++cycle)
        {
            rs.free();
            for(size_t i=ran.leq(10)+1;i>0;--i)
            {
                const T tmp = bring::get<T>(ran);
                rs.insert(tmp,ran.leq(4)+1);
            }
            std::cerr << rs << std::endl;
        }

    }

}
YACK_UTEST(data_roster)
{
    randomized::rand_ ran;

    perform<short>(ran);
    perform<string>(ran);


}
YACK_UDONE()


