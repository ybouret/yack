
#include "yack/utest/run.hpp"
#include "yack/counting/comb.h"
#include "yack/sequence/vector.hpp"
#include "yack/container/matrix.hpp"
#include "yack/apex/natural.hpp"
#include "yack/sequence/list.hpp"

using namespace yack;

YACK_UTEST(counting_comb)
{
    
    {
        size_t n=8;
        size_t k=5;
        
        if(argc>2)
        {
            n = atol(argv[1]);
            k = atol(argv[2]);
        }
        
        yack_comb       param;
        yack_comb_init(&param,n,k);
        std::cerr << "comb(" << n << "," << k << ")=" << param.count << std::endl;
        const apn nc = apn::comb(n,k);
        std::cerr << "=>" << nc << std::endl;
        
        YACK_CHECK(nc==param.count);
        
        
        if(param.count)
        {
            std::cerr << "generating..." << std::endl;
            {
                
                vector<size_t> comb(k,0);
                yack_comb_boot(&param,*comb);
#if 0
                for(size_t i=1;i<=param.count;++i)
                {
                    for(size_t j=k;j>0;--j)
                    {
                        frame[i][j] = comb[j];
                    }
                    yack_comb_next(&param,*comb);
                }
#endif
            }
            // std::cerr << frame << std::endl;
#if 0
            for(size_t i=1;i<param.count;++i)
            {
                const readable<size_t> &f_i = frame[i];
                for(size_t j=i+1;j<=param.count;++j)
                {
                    const readable<size_t> &f_j = frame[j];
                    YACK_ASSERT(f_i!=f_j);
                }
            }
#endif
        }
        
    }
    
    
    for(size_t n=1;;++n)
    {
        list<size_t> gt32;
        for(size_t k=1;k<=n;++k)
        {
            const apn    nc = apn::comb(n,k);
            const size_t nb = nc.bits();
            if(nb>32)
            {
                gt32.push_back(k);
            }
        }
        if(gt32.size())
        {
            std::cerr << "n=" << n << ": " << gt32.front() << " -> " << gt32.back() << std::endl;
        }
        
        if(n>=64)
            break;
    }
    
    
}
YACK_UDONE()

