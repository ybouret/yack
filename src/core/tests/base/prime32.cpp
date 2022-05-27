#include "yack/arith/prime.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/small/list.hpp"
#include "yack/arith/base2.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/arith/ilog2.hpp"

#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
#include "yack/fs/local/fs.hpp"

#include "yack/concurrent/loop/simd.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/gz/ostream.hpp"
#include "yack/container/matrix.hpp"

using namespace yack;



class engine : public concurrent::runnable
{
public:

    static const uint32_t  lower = 0xfff1;
    static const uint32_t  upper = 0xffffffff;
    static const size_t    cache = 16384;

    virtual ~engine() throw()
    {
    }



    explicit engine(const size_t n) : concurrent::runnable(),
    filename(n,as_capacity),
    pmatrix(n,cache)
    {
        for(size_t i=1;i<=n;++i)
        {
            const string fn = vformat("primes%u.%u.txt.gz",(unsigned)n,(unsigned)i);
            filename.push_back_fast(fn);
        }
    }

    virtual void run(const concurrent::context &ctx, lockable &sync) throw()
    {
        const size_t        i      = ctx.indx;
        writable<uint32_t> &primes = pmatrix[i];
        
    }

    vector<const string> filename;
    matrix<uint32_t>     pmatrix;
};


YACK_UTEST(prime32)
{


    const concurrent::topology topo;
    concurrent::simd           para(topo);




#if 0
    vfs &fs = localFS::instance();
    uint64_t p    = 0;
    size_t   n32  = 0;
    uint32_t prev = 0xfff1;
    uint32_t smax = 0x0;
    size_t   ppl  = 0;


    const string savename = "prime32.dat";
    const string smaxname = "s_max32.dat";

    if( fs.get_attr_of(savename) == vfs::entry::attr_reg )
    {
        throw exception("%s exists",savename());
    }


    if( fs.get_attr_of(smaxname) == vfs::entry::attr_reg )
    {
        throw exception("%s exists",savename());
    }

    ios::ocstream::overwrite(savename);
    ios::ocstream::overwrite(smaxname);

    while(true)
    {
        p = prime64::next(++p);
        const size_t nb = bits_for(p);
        if(nb>16)
        {

            if(nb>32) break;
            assert(nb<=32);
            ++n32;
            const uint32_t curr  = static_cast<uint32_t>(p);
            const uint32_t delta = curr-prev;  YACK_ASSERT(0==(delta&0x01));
            const uint32_t hdiff = delta/2;    YACK_ASSERT(hdiff>0);
            const uint32_t shift = hdiff-1;
            if(shift>smax)
            {
                smax=shift;
                ios::ocstream::echo(smaxname,"%u %u\n",(unsigned)n32,(unsigned)(smax));
            }
            ios::ocstream::echo(savename,"%u %u %u\n",(unsigned)n32, (unsigned)p, (unsigned)(shift) );

            if(0==(n32%10000))
            {
                (std::cerr << "|0x" << ios::hexa(curr,true)).flush();
                if(++ppl>=10)
                {
                    std::cerr << std::endl;
                    ppl = 0;
                }
            }

            prev = curr;
        }
    }
    ios::ocstream::echo(smaxname,"%u %u\n",(unsigned)n32,(unsigned)(smax));
    std::cerr << std::endl;
    std::cerr << "n32   = " << n32  << " <= 0x" << ios::hexa(prev,true) << std::endl;
    std::cerr << "smax  = " << smax << std::endl;
#endif


}
YACK_UDONE()
