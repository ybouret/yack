#include "yack/container/matrix.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_matrix)
{
    randomized::rand_ ran;

    { matrix<int> M; }
    {
        matrix<float> M(1,1);
        std::cerr << "allocated: " << M.allocated << std::endl;
    }

    {
        matrix<double> M(256,256);
        std::cerr << "allocated: " << M.allocated << std::endl;

        for(size_t r=1;r<=M.rows;++r)
        {
            matrix_row<double> &M_r = M[r];
            for(size_t c=1;c<=M.cols;++c)
            {
                M_r[c] = ran();
            }
        }
    }
    
    
}
YACK_UDONE()

