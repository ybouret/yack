#include "yack/container/matrix.hpp"
#include "yack/apex/natural.hpp"
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

    {
        matrix<apn> M(10,12);
        std::cerr << "allocated: " << M.allocated << std::endl;
        for(size_t r=1;r<=M.rows;++r)
        {
            matrix_row<apn> &M_r = M[r];
            for(size_t c=1;c<=M.cols;++c)
            {
                M_r[c] = apn(ran,ran.leq(1000));
            }
        }

        matrix<apn> P(M);
        for(size_t r=1;r<=M.rows;++r)
        {
            for(size_t c=1;c<=M.cols;++c)
            {
                YACK_ASSERT( M[r][c] == P[r][c] );
            }
        }

        {
            matrix<apn> Q;
            Q.xch(P);
        }

        matrix<apn> Q(M,transposed);
        YACK_CHECK(Q.cols==M.rows);
        YACK_CHECK(Q.rows==M.cols);
        for(size_t r=1;r<=M.rows;++r)
        {
            for(size_t c=1;c<=M.cols;++c)
            {
                YACK_ASSERT( M[r][c] == Q[c][r] );
            }
        }


    }
    
    
}
YACK_UDONE()

