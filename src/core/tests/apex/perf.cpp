
#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/endian.hpp"
#include "yack/type/complex.hpp"
#include "yack/system/wtime.hpp"

using namespace yack;

static inline apn gen_apn( uprng &ran, size_t ibits)
{
    if(ibits<=0)
    {
        return apn(0);
    }
    else
    {
        apn res=1;
        while(--ibits>0)
        {
            res <<= 1;
            if(ran.choice()) res += 1;
        }
        return res;
    }

}

YACK_UTEST(apex_perf)
{

    uprng ran;
    const size_t max_bits = 2048;
    wtime        chrono;

    std::cerr << "[MUL]" << std::endl;
    for(size_t bits=1;bits<=max_bits;bits<<=1)
    {
        std::cerr << "bits=" << std::setw(6) << bits << " :";
        apex::number::reset_tracking();
        assert(apex::number::fmul_ticks<=0);
        assert(apex::number::lmul_ticks<=0);

        uint64_t t64 = 0;
        do
        {
            const uint64_t mark = wtime::ticks();
            const apn lhs   = gen_apn(ran,bits); YACK_ASSERT(lhs.bits()==bits);
            const apn rhs   = gen_apn(ran,bits); YACK_ASSERT(rhs.bits()==bits);
            const apn lprod = apn::_lmul(lhs,rhs);
            const apn fprod = apn::_fmul(lhs,rhs);
            YACK_ASSERT(lprod==fprod);
            t64 += wtime::ticks() - mark;
        } while( chrono(t64)<0.5 );

        std::cerr << " lmul_rate: " << std::setw(15) << apex::number::lmul_rate();
        std::cerr << " fmul_rate: " << std::setw(15) << apex::number::fmul_rate();
        std::cerr << std::endl;
    }


}
YACK_UDONE()

