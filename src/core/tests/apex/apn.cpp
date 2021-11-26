
#include "yack/apex/natural.hpp"
#include "yack/arith/base2.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/endian.hpp"
#include "yack/randomized/rand.hpp"

using namespace yack;


YACK_UTEST(apn)
{

    randomized::rand_  ran;

    
    
    for(size_t bits=0;bits<=64;++bits)
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const apex::uint_type u = ran.gen<apex::uint_type>(bits);
            YACK_ASSERT(bits==bits_for(u));
            apn n = u;
            //std::cerr << std::hex << "u=" << u << std::dec << std::endl;
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

            apn m = 0;
            m = u;
            YACK_ASSERT(m.lsu()==u);
        }
    }
    std::cerr << std::dec;
    
    {
        apex::uint_type u = ran.gen<apex::uint_type>(40);
        size_t           n = 0;
        size_t           b = 0;
        std::cerr << std::hex << "u=" << u << std::endl;
        const apn::word_type *w = apn::u2w(u,n,b);
        std::cerr << "w[" << n << "]={";
        for(size_t i=n;i>0;--i)
        {
            std::cerr << ' ' << w[i-1];
        }
        std::cerr << "} #=" << b << "\n";
    }

    std::cerr << std::dec;

    YACK_SIZEOF(apn);
    std::cerr << "BE=" << endian::BE() << std::endl;
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

    std::cerr << "[RAN]" << std::endl;
    for(size_t nbit=0;nbit<=20000;++nbit)
    {
        const apn n(ran,nbit);
        YACK_ASSERT(n.bits()==nbit);
    }


    {
        std::cerr << "dec:" << std::endl;
        const apn n0 = apn::parse_dec("");  std::cerr << "n0=" << n0 << std::endl;
        const apn n2 = apn::parse_dec("2"); std::cerr << "n2=" << n2 << std::endl;
        const apn nn = apn::parse_dec("12345678901234567890");
        std::cerr << "nn=" << nn << std::endl;
    }
    {
        std::cerr << "hex:" << std::endl;
        const apn n0 = apn::parse_hex("");  std::cerr << "n0=" << n0 << std::endl;
        const apn n2 = apn::parse_hex("C"); std::cerr << "n2=" << n2 << std::endl;
        n2.output_hex(std::cerr << "\t") << std::endl;
        const apn nn = apn::parse_hex("1234567890ABCDEFabcdef");
        std::cerr << "nn=" << nn << std::endl;
        nn.output_hex(std::cerr << "\t") << std::endl;

    }
    
    
    std::cerr << "[CMP]" << std::endl;
    for(size_t ibits=0;ibits<=60;++ibits)
    {
        for(size_t jbits=0;jbits<=60;++jbits)
        {
            for(size_t iter=0;iter<32;++iter)
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
    for(size_t ibits=0;ibits<=60;++ibits)
    {
        for(size_t jbits=0;jbits<=60;++jbits)
        {
            for(size_t iter=0;iter<32;++iter)
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
    for(size_t ibits=0;ibits<=60;++ibits)
    {
        for(size_t jbits=0;jbits<=60;++jbits)
        {
            for(size_t iter=0;iter<32;++iter)
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

    std::cerr << "[MUL]" << std::endl;
    for(size_t ibits=0;ibits<=31;++ibits)
    {
        for(size_t jbits=0;jbits<=31;++jbits)
        {
            for(size_t iter=0;iter<128;++iter)
            {
                const apex::uint_type u = ran.gen<apex::uint_type>(ibits);
                const apex::uint_type v = ran.gen<apex::uint_type>(jbits);
                const apex::uint_type p = u*v;
                const apn U=u;
                const apn V=v;
                {
                    const apn P=U*V; YACK_ASSERT(P.lsu()==p);
                }
                {
                    const apn P=U*v; YACK_ASSERT(P.lsu()==p);
                }
                {
                    const apn P=u*V; YACK_ASSERT(P.lsu()==p);
                }
                {
                    apn P=u; P *= V;YACK_ASSERT(P.lsu()==p);
                }
                {
                    apn P=u; P *= v;YACK_ASSERT(P.lsu()==p);
                }
            }

        }

    }

    std::cerr << "[BIN_OUT]" << std::endl;
    for(size_t i=0;i<=256;++i)
    {
        const apn I = i;
        I.output_bin(std::cerr << " " << i << "=>");
        if( 0 == ((1+i)%8)) std::cerr << std::endl;
    }
    std::cerr << std::endl;

    std::cerr << "[BIN_EXP2]" << std::endl;
    for(size_t i=0;i<=70;++i)
    {
        apn I = apn::exp2(i);
        //std::cerr << "2^" << i << " = ";// I.output_bin(std::cerr) << " | ";
        //I.output_hex(std::cerr << "0x") << std::endl;
        YACK_ASSERT(I.bits()==i+1);
        size_t j=i+1;
        while(I>0)
        {
            //std::cerr << I.shr().bits() << std::endl;
            YACK_ASSERT(I.shr().bits() == --j);
        }
    }

    std::cerr << "[BIN_SHR]" << std::endl;
    for(size_t ibits=0;ibits<=63;++ibits)
    {
        for(size_t iter=0;iter<128;++iter)
        {
            apex::uint_type u = ran.gen<apex::uint_type>(ibits);
            apn             U = u;
            YACK_ASSERT( U.shr().lsu() == (u>>1) );
        }

        for(size_t iter=0;iter<128;++iter)
        {
            const apex::uint_type u = ran.gen<apex::uint_type>(ibits);
            const apn             U = u;
            for(size_t shift=0;shift<=ibits;++shift)
            {
                const apn             V = U.shr(shift);
                const apex::uint_type v = u>>shift;
                YACK_ASSERT(v==V);
            }
        }
    }

    std::cerr << "[BIN_SHL]" << std::endl;
    for(size_t ibits=0;ibits<=64;++ibits)
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const apex::uint_type u = ran.gen<apex::uint_type>(ibits);
            apn                   U = u;
            for(size_t shift=0;shift<=100;++shift)
            {
                apn V = U.shl(shift);
                V     = V.shr(shift);
                YACK_ASSERT(V==U);
            }
        }
    }

    //ios::ocstream::overwrite("divp.dat");


    std::cerr << "[DIV/MOD]" << std::endl;
    for(size_t nbit=0;nbit<=64;++nbit)
    {
        for(size_t dbit=1;dbit<=nbit;++dbit)
        {
            for(size_t iter=0;iter<2;++iter)
            {
                const apex::uint_type u = ran.gen<apex::uint_type>(nbit);
                const apex::uint_type v = ran.gen<apex::uint_type>(dbit);
                const apex::uint_type q = u/v;
                const apex::uint_type m = u%v;
                const apn U=u;
                const apn V=v;
                {
                    const apn Q=U/V; YACK_ASSERT(Q.lsu()==q);
                    const apn M=U%V; YACK_ASSERT(M.lsu()==m);

                    apn rem = 0;
                    apn quot = apn::quot(U,V,rem);
                    YACK_ASSERT(Q==quot);
                    YACK_ASSERT(M==rem);
                }
                {
                    const apn Q=U/v; YACK_ASSERT(Q.lsu()==q);
                    const apn M=U%v; YACK_ASSERT(M.lsu()==m);
                }
                {
                    const apn Q=u/V; YACK_ASSERT(Q.lsu()==q);
                    const apn M=u%V; YACK_ASSERT(M.lsu()==m);
                }
                {
                    apn Q=u; Q /= V; YACK_ASSERT(Q.lsu()==q);
                    apn M=u; M %= V; YACK_ASSERT(M.lsu()==m);
                }
                {
                    apn Q=u; Q /= v; YACK_ASSERT(Q.lsu()==q);
                    apn M=u; M %= v; YACK_ASSERT(M.lsu()==m);
                }
            }
        }
    }

   

    std::cerr << "[BITWISE]" << std::endl;
    for(size_t nbit=0;nbit<=64;++nbit)
    {
        apn n(ran,nbit);
        n.output_hex(std::cerr) << " => ";
        n &= 0xff;
        n.output_hex(std::cerr) << std::endl;
    }
    
    std::cerr << "add_rate : " << apex::number::add_rate() << std::endl;
    std::cerr << "sub_rate : " << apex::number::sub_rate() << std::endl;
    std::cerr << "lmul_rate: " << apex::number::lmul_rate() << std::endl;
    std::cerr << "div_rate:  " << apex::number::div_rate() << std::endl;
    std::cerr << "mod_rate:  " << apex::number::mod_rate() << std::endl;
}
YACK_UDONE()
