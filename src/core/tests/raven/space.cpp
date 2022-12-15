#include "yack/raven/qmatrix.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/utest/run.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/container/matrix.hpp"


using namespace yack;

namespace
{
    static inline void create(writable<int> &cf, randomized::bits &ran)
    {
        for(size_t i=cf.size();i>0;--i)
        {
            cf[i] = static_cast<int>( ran.in(-4,4) );
        }
    }

    static inline void create(matrix<int> &nu, randomized::bits &ran)
    {
        for(size_t i=nu.rows;i>0;--i)
        {
            create(nu[i],ran);
        }
    }


    static inline void test(const size_t size,
                            const size_t rank,
                            randomized::bits &ran)
    {
        raven::qmatrix U(size,rank);
        matrix<int>    nu(rank,size);
        do
        {
            create(nu,ran);
        } while( apk::rank_of(nu) < rank );

        std::cerr << "nu=" << nu << std::endl;
        for(size_t i=1;i<=rank;++i)
        {
            YACK_ASSERT(U.grow(nu[i]));
        }
        std::cerr << "U=" << U << std::endl;


    }


}

YACK_UTEST(raven_space)
{
    randomized::rand_ ran;

    test(7,3,ran);


    YACK_SIZEOF(raven::qvector);
    YACK_SIZEOF(raven::qmatrix);


}
YACK_UDONE()
