#include "yack/arith/prime.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/small/list.hpp"
#include "yack/arith/base2.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/arith/ilog2.hpp"
#include "yack/arith/prime-pi.hpp"

#include "yack/ios/ocstream.hpp"
#include "yack/string.hpp"
#include "yack/fs/local/fs.hpp"

#include "yack/concurrent/loop/simd.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/container/matrix.hpp"


using namespace yack;
using namespace concurrent;

class engine : public runnable
{
public:
    static const uint64_t lower = 65536;
    static const uint64_t po2   = 32;
    static const uint64_t one   = 1;
    static const uint64_t upper = one << po2;
    static const uint64_t width = (upper-lower)+1;
    static const size_t   cache = 32768;
    
    virtual ~engine() throw()
    {
    }
    
    explicit engine(const string &root, const size_t n) :
    runnable(),
    output(0),
    pmatrix(n,cache),
    filename(n,as_capacity),
    total(n,0),
    ini(n,0),
    end(n,0)
    {
        std::cerr << "<engine>" << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            char buf[64];
            context::format(buf,sizeof(buf),n,i);
            string fn = root;
            fn += buf;
            fn += ".dat";
            filename.push_back_fast(fn);
            
            vfs &fs = localFS::instance();
            if( fs.is_reg(fn) ) throw exception("%s exists!!",fn());
            
            std::cerr << "  <" << fn << ">" << std::endl;
            ios::ocstream::overwrite(fn);
        }
        std::cerr << "<engine/>" << std::endl;
    }
    
    void flush(size_t &count, const context &ctx, lockable &sync)
    {
        char buf[64];
        ctx.format(buf,sizeof(buf));
        const size_t              i = ctx.indx;
        const readable<uint32_t> &m = pmatrix[i];
        const string             &f = filename[i];
        YACK_LOCK(sync);
        if(count)
        {
            (std::cerr << "|" << buf << "@" << ios::hexa(m[count],true)).flush();
            if(++output>=8)
            {
                std::cerr << std::endl;
                output=0;
            }
            
            ios::ocstream fp(f,true);
            for(size_t j=1;j<=count;++j)
            {
                fp("%lu\n",static_cast<unsigned long>(m[j]));
            }
            total[i] += count;
            count = 0;
        }
    }
    
    virtual void run(const context &ctx,
                     lockable      &sync) throw()
    {
        const size_t        i      = ctx.indx;
        writable<uint32_t> &m      = pmatrix[i];
        uint64_t            offset = lower;
        uint64_t            length = width;
        ctx.crop(length,offset);
        {
            const uint64_t last = offset+length-1;
            YACK_LOCK(sync);
            std::cerr << "@" << i << " : " << offset << " -> " << last << " #" << length << std::endl;
        }
        
        ini[i] = offset;
        
        while(length>0 && 0==(offset&0x1))
        {
            --length;
            ++offset;
        }
        
        size_t count = 0;
        while(length>0)
        {
            if(bits_for(offset)>32)
            {
                break;
            }
            end[i] = offset;
            if(0!=(offset&0x1))
            {
                if(prime64::is(offset))
                {
                    m[++count] = uint32_t(offset);
                    if(count>=cache) flush(count,ctx,sync);
                }
            }
            ++offset;
            --length;
        }
        flush(count,ctx,sync);
    }
    
    size_t               output;
    matrix<uint32_t>     pmatrix;
    vector<const string> filename;
    vector<size_t>       total;
    vector<uint64_t>     ini;
    vector<uint64_t>     end;
    
private:
    YACK_DISABLE_COPY_AND_ASSIGN(engine);
};


YACK_UTEST(prime32)
{

  
    if(argc<=1)
    {
        std::cerr << "need a root file name" << std::endl;
        return 0;
    }
    
    const string   root = argv[1];
    const topology topo;
    simd           para(topo);
    engine         ex32(root,para.size());
    
    para.run(ex32);
    std::cerr << std::endl;
    size_t total = 0;
    for(size_t i=1;i<=ex32.total.size();++i)
    {
        std::cerr << "cpu#" << std::setw(3) << i;
        std::cerr << " " << std::setw(10) << ex32.ini[i] << " -> " <<  std::setw(10) << ex32.end[i];
        std::cerr << " : found #" << ex32.total[i];
        std::cerr << std::endl;
        total += ex32.total[i];
    }

    const size_t all = total + core::primes::pi16;
    std::cerr << "found  #" << all << " = " << total << "+" << core::primes::pi16 << std::endl;
    std::cerr << "stored #" << core::prime_pi::table[engine::po2] << std::endl;
    if(all!=core::prime_pi::table[engine::po2])
        throw exception("count mismatch!!");
    std::cerr << "[DONE]" << std::endl;
}
YACK_UDONE()
