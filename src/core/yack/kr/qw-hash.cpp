#include "yack/kr/qw-hash.hpp"
#include <cstdio>

namespace yack
{
    
    namespace crypto
    {
        
        void hash64::des(uint32_t &lword, uint32_t &irword) throw()
        {
             
            static const unsigned  CYCLES=4;
            static uint32_t c1[CYCLES]={ 0xbaa96887L, 0x1e17d32cL, 0x03bcdc3cL, 0x0f33d1b2L};
            static uint32_t c2[CYCLES]={ 0x4b0f3b58L, 0xe874f0c3L, 0x6955c5a6L, 0x55a7ca46L};
            
            for(unsigned i=0;i<CYCLES;++i)
            {
                const uint32_t iswap  = irword;
                uint32_t       ia    = irword ^ c1[i];
                const uint32_t itmpl = ia & 0xffff;
                const uint32_t itmph = ia >> 16;
                const uint32_t ib    = itmpl*itmpl+ ~(itmph*itmph);
                irword = lword ^ (((ia = (ib >> 16) | ((ib & 0xffff) << 16)) ^ c2[i])+itmpl*itmph);
                lword  = iswap;
            }
            
        }
        
        bool hash64:: des_test()
        {
            static const uint32_t input[4][2] =
            {
                {1,  1},
                {1, 99},
                {99, 1},
                {99,99}
            };
            
            static const uint32_t output[4][2] =
            {
                {0x604D1DCE, 0x509C0C23},
                {0xD97F8571, 0xA66CB41A},
                {0x7822309D, 0x64300984},
                {0xD7F376F0, 0x59BA89EB}
            };
            
            for(size_t i=0;i<4;++i)
            {
                uint32_t lo = input[i][0];
                uint32_t hi = input[i][1];
                des(lo,hi);
                printf("%08x | %08x\n",lo,output[i][0]);
                printf("%08x | %08x\n",hi,output[i][1]);
                if(output[i][0]!=lo || output[i][1]!=hi)
                    return false;
            }
            
            return true;
        }

    }
    
}
