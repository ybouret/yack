#include "yack/container/matrix.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_matrix)
{
    
    { matrix<int> M; }
    {
        matrix<float> M(1,1);
        std::cerr << "allocated: " << M.allocated << std::endl;
    }
    
    {
        matrix<double> M(256,256);
        std::cerr << "allocated: " << M.allocated << std::endl;
    }
    
    
}
YACK_UDONE()

