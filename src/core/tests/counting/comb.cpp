
#include "yack/utest/run.hpp"
#include "yack/counting/comb.h"
#include "yack/sequence/vector.hpp"
#include "yack/container/matrix.hpp"

using namespace yack;

YACK_UTEST(counting_comb)
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

    if(param.count)
    {
        matrix<size_t> frame(param.count,k);
        {

            vector<size_t> comb(k,0);
            yack_comb_boot(&param,*comb);
            for(size_t i=1;i<=param.count;++i)
            {
                std::cerr << "comb=" << comb << std::endl;
                for(size_t j=k;j>0;--j)
                {
                    frame[i][j] = comb[j];
                }
                yack_comb_next(&param,*comb);
            }
        }
        std::cerr << frame << std::endl;
        for(size_t i=1;i<param.count;++i)
        {
            const readable<size_t> &f_i = frame[i];
            for(size_t j=i+1;j<=param.count;++j)
            {
                const readable<size_t> &f_j = frame[j];
                YACK_ASSERT(f_i!=f_j);
            }
        }
    }


    

}
YACK_UDONE()

