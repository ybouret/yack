#include "yack/container/matrix.hpp"
#include "yack/apex/natural.hpp"
#include "yack/utest/run.hpp"

#include <typeinfo>

using namespace yack;
namespace
{

    class dummy
    {
    public:
        size_t        rank;
        static size_t count;

        inline  dummy() noexcept : rank(1) { ++count; }
        inline ~dummy() noexcept { --count; }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dummy);
    };

    size_t dummy::count = 0;

    template <typename T> static inline
    void run_matrix( matrix<T> &M )
    {
        std::cerr << std::endl;
        std::cerr << "matrix<" << typeid(T).name() << ">" << std::endl;
        std::cerr << "  |_(" << M.rows << "x" << M.cols << ")" << std::endl;
        std::cerr << "  |_granted  : " << M.granted() << std::endl;
        std::cerr << "  |_stride   : " << M.stride    << std::endl;
    }


}



YACK_UTEST(data_matrix)
{
    randomized::rand_ ran;

    YACK_SIZEOF(matrix<char>);
    YACK_SIZEOF(matrix<int>);
    YACK_SIZEOF(matrix<uint64_t>);
    YACK_SIZEOF(matrix<dummy>);



    {
        matrix<int> M;
        run_matrix(M);
        std::cerr << "M=" << M << std::endl;
    }
    {
        matrix<float> M(1,1);
        M.ld(1.0f);
        run_matrix(M);
        std::cerr << "M=" << M << std::endl;
    }

    {
        matrix<double> M(16,16);
        M.ld(-1.0);
        run_matrix(M);
        std::cerr << "M=" << M << std::endl;

        for(size_t r=1;r<=M.rows;++r)
        {
            matrix_row<double> &M_r = M[r];
            for(size_t c=1;c<=M.cols;++c)
            {
                M_r[c] = ran();
            }
        }
        std::cerr << "M=" << M << std::endl;

    }
    

    {
        matrix<apn> M(10,15);
        run_matrix(M);
        std::cerr << "M=" << M << std::endl;
        M.ld(1);
        std::cerr << "M=" << M << std::endl;
        for(size_t r=1;r<=M.rows;++r)
        {
            matrix_row<apn> &M_r = M[r];
            for(size_t c=1;c<=M.cols;++c)
            {
                M_r[c] = apn(ran,ran.leq(20));
            }
        }
        std::cerr << "M=" << M << std::endl;


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
        std::cerr << "Mt=" << Q << std::endl;
        YACK_CHECK(Q.cols==M.rows);
        YACK_CHECK(Q.rows==M.cols);
        for(size_t r=1;r<=M.rows;++r)
        {
            for(size_t c=1;c<=M.cols;++c)
            {
                YACK_ASSERT( M[r][c] == Q[c][r] );
            }
        }

        for(size_t iter=0;iter<=16;++iter)
        {
            const size_t r1 =1+ran.leq(M.rows-1);
            const size_t r2 =1+ran.leq(M.rows-1);
            std::cerr << "swap_rows(" << r1 << "," << r2 << ")" << std::endl;
            M.swap_rows(r1,r2);
        }


    }


    {
        matrix<double> a(10,10);
        matrix<int>    d(10,10);
        a.assign(d);
    }

    
}
YACK_UDONE()

