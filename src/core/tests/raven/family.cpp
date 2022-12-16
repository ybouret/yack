
#include "yack/raven/qbranch.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/utest/run.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/container/matrix.hpp"
#include "yack/ios/ascii/convert.hpp"

#include "yack/counting/perm.hpp"
#include "yack/sequence/roll.hpp"


using namespace yack;

namespace
{
    static inline void create(writable<int> &cf, randomized::bits &ran)
    {
        for(size_t i=cf.size();i>0;--i)
        {
            cf[i] = static_cast<int>( ran.in(-3,3) );
        }
    }

    static inline void create(matrix<int> &nu, randomized::bits &ran)
    {
        for(size_t i=nu.rows;i>0;--i)
        {
            create(nu[i],ran);
        }
    }

    static inline
    bool accept_all(const readable<int> &mu)
    {
        std::cerr << "initialize with " << mu << std::endl;
        return true;
    }

    static inline
    void display_vec(const readable<apz> &cf)
    {
        std::cerr << " -> " << cf << std::endl;
    }

    static inline void test(const size_t      size,
                            const size_t      rank,
                            randomized::bits &ran)
    {

        std::cerr << "Testing with size=" << size << " | rank=" << rank << std::endl;


        // building
        matrix<int>    nu(rank,size);
        do
        {
            create(nu,ran);
        } while( apk::rank_of(nu) < rank );
        std::cerr << "nu=" << nu << std::endl;

        matrix<int> mu;
        raven::qselect::compress(mu,nu);
        std::cerr << "mu=" << mu << std::endl;
        YACK_CHECK(apk::rank_of(mu) == rank);



        raven::qbranch source;
        source(mu,rank,accept_all);

        std::cerr << source << std::endl;

        while(source.generate(mu,display_vec))
        {
            std::cerr << source << std::endl;
        }




    }

}


YACK_UTEST(raven_family)
{
    randomized::rand_ ran;
    size_t size = 5; if(argc>1) size = ios::ascii::convert::to<size_t>(argv[1]);
    size_t rank = 3; if(argc>2) rank = ios::ascii::convert::to<size_t>(argv[2]);


    test(size,rank,ran);

    YACK_SIZEOF(raven::qmatrix);
    YACK_SIZEOF(raven::qfamily);


}
YACK_UDONE()

