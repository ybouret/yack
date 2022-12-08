#include "yack/apex/worthy.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/data/small/set.hpp"
#include "../main.hpp"

using namespace yack;

static inline void create_vec(writable<int> &v, randomized::bits &ran)
{
    for(size_t i=v.size();i>0;--i)
    {
        v[i] = static_cast<int>( ran.in(-20,20) );
    }
}

static inline void create_end(writable<apq> &v, const worthy::qfamily &U, randomized::bits &ran)
{
    YACK_ASSERT(worthy::almost_done==U.situation);
    cxx_array<int>  any(v.size());
    worthy::qfamily V(U);
    do
    {
        create_vec(any,ran);
    }
    while( !V.grow(any) );

    for(size_t i=v.size();i>0;--i)
    {
        v[i] = V->tail->coef[i];
    }

}

YACK_UTEST(apex_worthy)
{
    randomized::rand_ ran;


    {
        worthy::qshared cache = new worthy::qcache(3);

        {
            worthy::qfamily U(cache);
            worthy::qfamily V(cache);





            matrix<int> vec(3,3);
            vec[1][1] = 1;  vec[1][2] = 2;  vec[1][3] = 3;
            vec[2][1] = 0;  vec[2][2] = 5;  vec[2][3] = 7;
            vec[3][1] = 11; vec[3][2] = 13; vec[3][3] = 0;




            YACK_CHECK(U.grow(vec[1]));
            std::cerr << U.project(vec[1]) << std::endl;
            YACK_CHECK(U.grow(vec[2]));
            std::cerr << U.project(vec[1]) << std::endl;
            std::cerr << U.project(vec[2]) << std::endl;
            YACK_CHECK(U.grow(vec[3]));
            std::cerr << U.project(vec[1]) << std::endl;
            std::cerr << U.project(vec[2]) << std::endl;
            std::cerr << U.project(vec[3]) << std::endl;

            std::cerr << U << std::endl;

            YACK_CHECK(V.grow(vec[3]));
            YACK_CHECK(V.grow(vec[2]));
            YACK_CHECK(V.grow(vec[1]));

            std::cerr << V << std::endl;

            V.reset();
            YACK_CHECK(V.grow(vec[1])); YACK_CHECK(!(V==U));
            YACK_CHECK(V.grow(vec[2])); YACK_CHECK(!(V==U));
            YACK_CHECK(V.grow(vec[3])); YACK_CHECK( (V==U));




            vector<int>       any(3);
            for(size_t iter=0;iter<8;++iter)
            {
                bring::fill(any,ran);
                YACK_CHECK(!U.grow(any));
            }

            std::cerr << "Making Last Line" << std::endl;
            matrix<apq> res(10,3);
            for(size_t i=1;i<=res.rows;++i)
            {
                U.reset();
                YACK_ASSERT(U.grow(vec[1]));
                YACK_ASSERT(U.grow(vec[2]));

                do
                {
                    bring::fill(any,ran);
                } while(!U.grow(any));

                for(size_t j=res.cols;j>0;--j)
                {
                    res[i][j] = U->tail->coef[j];
                }
            }
            std::cerr << res << std::endl;
        }
        std::cerr << "|cache|=" << (*cache)->size << std::endl;
    }

    for(size_t dims=2;dims<=5;++dims)
    {

        std::cerr << "-------- #dim=" << dims << " --------" << std::endl;
        worthy::qshared cache = new worthy::qcache(dims);
        worthy::qfamily U(cache); YACK_ASSERT(dims==U.dimension);
        cxx_array<int>  v(dims);

        cxx_array<apq>  last(dims);
        cxx_array<apq>  temp(dims);

        // create up to dims-1
        for(size_t i=1;i<dims;++i)
        {
            do
            {
               create_vec(v,ran);
            } while( !U.grow(v) );
        }

        std::cerr << "U=" << U << std::endl;
        create_end(last,U,ran);
        std::cerr << "last=" << last << std::endl;

        for(size_t iter=0;iter<100;++iter)
        {
            create_end(temp,U,ran);
            YACK_ASSERT(temp==last);
        }

    }

    YACK_SIZEOF(worthy::coefficients);
    YACK_SIZEOF(worthy::qarray);
    YACK_SIZEOF(worthy::qfamily);

    
}
YACK_UDONE()

