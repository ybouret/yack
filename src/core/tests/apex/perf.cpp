
#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/endian.hpp"
#include "yack/type/complex.hpp"
#include "yack/system/wtime.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/randomized/rand.hpp"
#include <cmath>

using namespace yack;

#include "yack/arith/align.hpp"

YACK_UTEST(apex_perf)
{

    double     tsample = 0.5;
    if(argc>1) tsample = atof(argv[1]);
    randomized::rand_  ran;
    const size_t       max_bytes = 8192;
    wtime              chrono;

    std::cerr << "[MUL]" << std::endl;
    ios::ocstream::overwrite("mul.dat");
    for(size_t bytes=1;bytes<=max_bytes;bytes<<=1)
    {
        std::cerr << "bytes=" << std::setw(6) << bytes;
        const size_t exp2 = integer_log2(bytes);
        apex::number::reset_tracking();
        assert(apex::number::fmul_ticks<=0);
        assert(apex::number::lmul_ticks<=0);

        uint64_t t64 = 0;
        do
        {
            const size_t   bits  = bytes * 8;
            const uint64_t mark  = wtime::ticks();
            const apn      lhs(ran,bits); YACK_ASSERT(lhs.bits()==bits);
            const apn      rhs(ran,bits); YACK_ASSERT(rhs.bits()==bits);
            const apn      lprod = apn::_lmul(lhs,rhs);
            const apn      fprod = apn::_fmul(lhs,rhs);
            YACK_ASSERT(lprod==fprod);
            if(lprod.bits()<=64)
            {
                const uint64_t l = lhs.lsu();
                const uint64_t r = rhs.lsu();
                const uint64_t p = l*r;
                YACK_ASSERT(lprod.lsu()==p);
            }
            t64 += wtime::ticks() - mark;
        } while( chrono(t64) < tsample );

        std::cerr << " | lmul_rate: " << std::setw(15) << apex::number::lmul_rate();
        std::cerr << " | fmul_rate: " << std::setw(15) << apex::number::fmul_rate();
        ios::ocstream::echo("mul.dat","%u %g %g\n",unsigned(exp2),log10(apex::number::lmul_rate()), log10(apex::number::fmul_rate()));
        std::cerr << std::endl;
    }

    std::cerr << "[SQR]" << std::endl;
    ios::ocstream::overwrite("sqr.dat");
    for(size_t bytes=1;bytes<=max_bytes;bytes<<=1)
    {
        std::cerr << "bytes=" << std::setw(6) << bytes;
        const size_t exp2 = integer_log2(bytes);
        apex::number::reset_tracking();
        assert(apex::number::fsqr_ticks<=0);
        assert(apex::number::lmul_ticks<=0);

        uint64_t t64 = 0;
        do
        {
            const size_t   bits  = bytes * 8;
            const uint64_t mark  = wtime::ticks();
            const apn      lhs(ran,bits); YACK_ASSERT(lhs.bits()==bits);
            const apn      lprod = apn::_lsqr(lhs);
            const apn      fprod = apn::_fsqr(lhs);
            YACK_ASSERT(lprod==fprod);
            if(lprod.bits()<=64)
            {
                const uint64_t l = lhs.lsu();
                const uint64_t p = l*l;
                YACK_ASSERT(lprod.lsu()==p);
            }
            t64 += wtime::ticks() - mark;
        } while( chrono(t64) < tsample );

        std::cerr << " | lsqr_rate: " << std::setw(15) << apex::number::lmul_rate();
        std::cerr << " | fsqr_rate: " << std::setw(15) << apex::number::fsqr_rate();
        ios::ocstream::echo("sqr.dat","%u %g %g\n",unsigned(exp2),log10(apex::number::lmul_rate()), log10(apex::number::fsqr_rate()));
        std::cerr << std::endl;
    }




}
YACK_UDONE()

