#include "yack/apex/primes.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/tokenizer.hpp"

using namespace yack;

namespace {

    static const unsigned first100[100] =
    {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541
    };

}

YACK_UTEST(aprimes)
{
    apex::primes & P = apex::primes::instance();
    std::cerr << P.call_sign << std::endl;

    YACK_SIZEOF(apn);
    YACK_SIZEOF(apex::prime_knot);

    if(false)
    {
        size_t count=0;
        for(const apex::prime_knot *node=P->head;node;node=node->next)
        {
            ++count;
            std::cerr << '#' << std::setw(5) << count << " : " <<  *node << std::endl;
        }
    }

    vector<apn>     prm(1024*1024,as_capacity);

    {
        for(size_t i=0;i<sizeof(first100)/sizeof(first100[0]);++i)
        {
            prm << first100[i];
        }
    }

    if(argc>1)
    {

        prm.free();
        {
            ios::icstream   fp(argv[1]);
            vector<string>  words;
            ios::characters line;
            while( fp.gets(line) )
            {
                words.free();
                const string l = line.to_string();
                tokenizer::split_with(' ',words,l);
                std::cerr << words << std::endl;
                for(size_t i=1;i<=words.size();++i)
                {
                    const apn n = apn::parse(words[i]());
                    prm << n;
                }
            }
        }

    }

    std::cerr << "#read: " << prm.size() << std::endl;
    {
        std::cerr << "[";
        size_t count=0;
        for(size_t i=1;i<=prm.size();++i)
        {
            const apn &p = prm[i];
            YACK_ASSERT(P.is(p));
            if(++count>=100)
            {
                (std::cerr << '.').flush();
                count = 0;
            }
        }
        std::cerr << "]" << std::endl;
    }
    //std::cerr << *P << std::endl;
    std::cerr << "#known=" << P->size << std::endl;



}
YACK_UDONE()

#include "yack/ios/ocstream.hpp"
#include "yack/ios/ascii/convert.hpp"



namespace
{
    static uint64_t emit_s8(const readable<uint8_t> &s8, const size_t bps)
    {
        uint64_t code = 0;
        for(size_t i=1;i<=s8.size();++i)
        {
            code <<= bps;
            code |= s8[i];
        }

        return code;
    }
}

#include "yack/ios/fmt/hexa.hpp"
#include "yack/system/endian.hpp"

YACK_UTEST(aprimes64)
{
    size_t       limit = 1024; if(argc>1) limit = ios::ascii::convert::to<size_t>(argv[1]);
    static const size_t word_size = 8;
    static const size_t word_bits = 64;
    const size_t bps    = 6;               //!< bits per shift
    const size_t spw    = word_bits/bps;   //!< shifts per word
    const size_t n64    = limit/word_size; //!< available words
    const size_t num    = n64 * spw;       //!< number of possible shifts

    std::cerr << "Encoding with : " <<  std::setw(5) << bps << " bits per shift" << std::endl;
    std::cerr << "Storing       : " <<  std::setw(5) << spw << " shifts per word" << std::endl;
    std::cerr << "Usable words  : " <<  std::setw(5) << n64 << " for " << limit << " bytes" << std::endl;
    std::cerr << "Usable shifts : " <<  std::setw(5) << num << std::endl;

    vector<uint8_t> s8(spw,as_capacity);

    // initialize
    apex::primes          & P    = apex::primes::instance();
    const apex::prime_knot *curr = P->head;
    curr = P.next(curr); YACK_CHECK(3==*curr);
    ios::ocstream fp("prime-db.inc");
    const size_t wpl = 4;
    size_t       cnt = 0;
    for(size_t idx = 1;idx<=num;++idx)
    {
        const apex::prime_knot *next = P.next(curr);
        const apn               delta = *next - *curr; YACK_ASSERT(delta.is_divisible_by(2) );
        const apn               shift = (delta/2)-1;   YACK_ASSERT(shift.bits()<=bps);
        s8 << shift.cast_to<uint8_t>();
        if(s8.size()>=spw)
        {
            const uint64_t  u64 = emit_s8(s8,bps);
            const ios::hexa x64(u64,true);
            std::cerr << "  0x" << x64 << std::endl;
            //ios::encoder::put(fp,u64);
            const union
            {
                uint64_t qw;
                uint32_t dw[2];
            } alias = { endian::swap_be(u64) };
            fp(" YACK_U64(0x%08x%08x)", endian::swap_be(alias.dw[0]), endian::swap_be(alias.dw[1]) );
            if(idx<num) fp << ',';
            if(++cnt>=wpl)
            {
                fp << '\n';
                cnt = 0;
            }
            s8.free();
        }
        curr = next;
    }
    YACK_ASSERT(0==s8.size());
    std::cerr << "up to " << *curr << std::endl;
    std::cerr << "use 'mv prime-db.inc src/core/yack/arith'" << std::endl;

}
YACK_UDONE()
