
#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/endian.hpp"
#include "yack/system/wtime.hpp"

using namespace yack;

namespace
{
    
}

YACK_UTEST(apex_apn)
{



    uprng ran;
    for(size_t bits=0;bits<=64;++bits)
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const apex::uint_type u = ran.gen<apex::uint_type>(bits);
            YACK_ASSERT(bits==bits_for(u));
            apn n = u;
            std::cerr << std::hex << "u=" << u << std::dec << std::endl;
            YACK_ASSERT(n.lsu()==u);
            YACK_ASSERT(bytes_for(u)==n.size());
            YACK_ASSERT(n.bits()==bits);

            apex::uint_type v = 0;
            for(size_t i=n.size();i>0;--i)
            {
                v <<= 8;
                v |= n[i];
            }
            YACK_ASSERT(u==v);
            std::cerr << "n=" << n << std::endl;

            apn m = 0;
            m = u;
            YACK_ASSERT(m.lsu()==u);

            
        }
    }
    std::cerr << std::dec;

    {
        apex::uint_type u = ran.gen<apex::uint_type>(40);
        size_t              n = 0;
        std::cerr << std::hex << "u=" << u << std::endl;
        const apn::word_type *w = apn::u2w(u,n);
        std::cerr << "w[" << n << "]={";
        for(size_t i=n;i>0;--i)
        {
            std::cerr << ' ' << w[i-1];
        }
        std::cerr << "}\n";
    }

    std::cerr << std::dec;
    YACK_SIZEOF(apn);
    std::cerr << "BE=" << endianness::BE() << std::endl;
    std::cerr << "apn::min_words_bytes = " << apn::min_words_bytes << std::endl;
    std::cerr << "apn::min_words_size  = " << apn::min_words_size  << std::endl;
    std::cerr << "apn::min_words_exp2  = " << apn::min_words_exp2  << std::endl;
    std::cerr << "apn::words_per_uint  = " << apn::words_per_uint  << std::endl;

    for(size_t i=0;i<=33;++i)
    {
        const size_t wexp2 = apn::words_exp2_for(i);
        const size_t wsize = size_t(1) << wexp2;
        YACK_ASSERT(wsize>=i);
    }

    std::cerr << "[CMP]" << std::endl;
    for(size_t ibits=0;ibits<=32;++ibits)
    {
        for(size_t jbits=0;jbits<=32;++jbits)
        {
            for(size_t iter=0;iter<64;++iter)
            {
                const apex::uint_type u = ran.gen<apex::uint_type>(ibits);
                const apn             U = u;
                const apex::uint_type v = ran.gen<apex::uint_type>(jbits);
                const apn             V = v;

                //std::cerr << "u=" << u << ", v=" << v << std::endl;
                //std::cerr << "U=" << U << ", V=" << V << std::endl;

#define YACK_TEST_CMP(OP) \
YACK_ASSERT( (u OP v) == (U OP V) ); \
YACK_ASSERT( (u OP v) == (u OP V) );\
YACK_ASSERT( (u OP v) == (U OP v) )

                YACK_TEST_CMP(<);
                YACK_TEST_CMP(<=);
                YACK_TEST_CMP(==);
                YACK_TEST_CMP(!=);
                YACK_TEST_CMP(>=);
                YACK_TEST_CMP(>);

            }

        }
    }

    std::cerr << "[ADD]" << std::endl;
    for(size_t ibits=0;ibits<=32;++ibits)
    {
        for(size_t jbits=0;jbits<=32;++jbits)
        {
            for(size_t iter=0;iter<128;++iter)
            {
                const apex::uint_type u = ran.gen<apex::uint_type>(ibits);
                const apn             U = u;
                const apex::uint_type v = ran.gen<apex::uint_type>(jbits);
                const apn             V = v;
                const apex::uint_type s = u+v;
                {
                    const apn S=U+V; YACK_ASSERT(S.lsu()==s);
                }
                {
                    const apn S=U+v; YACK_ASSERT(S.lsu()==s);
                }
                {
                    const apn S=u+V; YACK_ASSERT(S.lsu()==s);
                }
                {
                    apn S=u; S += V;YACK_ASSERT(S.lsu()==s);
                }
                {
                    apn S=u; S += v;YACK_ASSERT(S.lsu()==s);
                }
            }
        }

    }

    std::cerr << "[INC]" << std::endl;
    for(apn i=0;i<=32;++i)
    {
        std::cerr << i  << "/";
        apn       j = i;
        const apn k = j++;
        YACK_ASSERT(k<j);
    }
    std::cerr << std::endl;

    std::cerr << "[SUB]" << std::endl;
    for(size_t ibits=0;ibits<=32;++ibits)
    {
        for(size_t jbits=0;jbits<=32;++jbits)
        {
            for(size_t iter=0;iter<128;++iter)
            {
                apex::uint_type u = ran.gen<apex::uint_type>(ibits);
                apex::uint_type v = ran.gen<apex::uint_type>(jbits);
                if(v>u) cswap(u,v);
                assert(v<=u);
                const apex::uint_type d = u-v;
                const apn             U = u;
                const apn             V = v;
                {
                    const apn D=U-V; YACK_ASSERT(D.lsu()==d);
                }
                {
                    const apn D=U-v; YACK_ASSERT(D.lsu()==D);
                }
                {
                    const apn D=u-V; YACK_ASSERT(D.lsu()==D);
                }
                {
                    apn D=u; D -= V;YACK_ASSERT(D.lsu()==D);
                }
                {
                    apn D=u; D -= v;YACK_ASSERT(D.lsu()==D);
                }
            }
        }
    }

    std::cerr << "[DEC]" << std::endl;
    for(apn i=32;i>0;--i)
    {
        std::cerr << i  << "/";
        apn       j = i;
        const apn k = j--;
        YACK_ASSERT(k>j);
    }
    std::cerr << std::endl;

    wtime sw;
    std::cerr << "add_rate: " << apex::number::add_count/sw(apex::number::add_ticks)<< std::endl;
    std::cerr << "sub_rate: " << apex::number::sub_count/sw(apex::number::sub_ticks)<< std::endl;


    
}
YACK_UDONE()
