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


#if 0
    static inline void test(const size_t      eqs,
                            const size_t      spc,
                            randomized::bits &ran)
    {
        YACK_ASSERT(eqs>=2);
        YACK_ASSERT(spc>=eqs);

        // create topology
        matrix<int> nu(eqs,spc);
        do {
            create(nu,ran);
        } while( apk::rank_of(nu) < eqs );
        std::cerr << "nu=" << nu << std::endl;

        // compress trials
        matrix<int> mu;
        raven::qselect::compress(mu,nu);
        std::cerr << "mu=" << mu << std::endl;
        YACK_ASSERT(mu.cols==eqs);

        // create qmatrix
        raven::qmatrix U(eqs,eqs);

        std::cerr << U << std::endl;

        if( U.grow(mu[1]) )
        {
            std::cerr << U << std::endl;
        }

    }
#endif

    static inline void test(const size_t size,
                            const size_t rank,
                            randomized::bits &ran)
    {
        raven::qmatrix U(size,rank);
        vector<int>    v(size);

        // initial space
        do
        {
            create(v,ran);
        } while( !U.grow(v) );
        std::cerr << "init with " << v << std::endl;

        std::cerr << U << std::endl;
        while(U.current_rank<U.maximum_rank)
        {
            do
            {
                create(v,ran);
            } while( !U.grow(v) );
            std::cerr << "grow with " << v << std::endl;
            std::cerr << U << std::endl;
        }


    }


}

YACK_UTEST(raven_space)
{
    randomized::rand_ ran;

    test(3,2,ran);


    YACK_SIZEOF(raven::qvector);
    YACK_SIZEOF(raven::qmatrix);


}
YACK_UDONE()

