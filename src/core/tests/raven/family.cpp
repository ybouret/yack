
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

        if(false)
        {
            cxx_array<apz> urr(mu.cols);
            cxx_array<apz> vrr(mu.cols);

            for(size_t i=1;i<mu.rows;++i)
            {
                for(size_t j=i+1;j<=mu.rows;++j)
                {
                    raven::qmatrix U(mu.cols,rank);
                    YACK_ASSERT(U(mu[i]));
                    if(!U(mu[j])) continue;
                    std::cerr << "(" << i << "," << j << ")" << std::endl;

                    raven::qmatrix V(mu.cols,rank);
                    YACK_ASSERT(V(mu[j]));
                    YACK_ASSERT(V(mu[i]));
                    YACK_ASSERT(U.size()==V.size());

                    while(U.active_state!=raven::qmatrix::fully_grown)
                    {
                        std::cerr << "\tU=" << U << std::endl;
                        std::cerr << "\tV=" << V << std::endl;
                        YACK_ASSERT(U.is_equivalent_to(V));
                        for(size_t k=2;k<=mu.rows;++k)
                        {
                            const readable<int> &mu_k = mu[k];
                            if(U.guess(urr,mu_k))
                            {
                                YACK_ASSERT(V.guess(vrr,mu_k));
                                std::cerr << "\t\t (" << k <<") " << mu_k << " -> " << urr << " | " << vrr << std::endl;
                                YACK_ASSERT(urr==vrr);
                            }
                            else
                            {
                                YACK_ASSERT(!V.guess(vrr,mu_k));
                            }
                        }

                        for(size_t k=1;k<=mu.rows;++k)
                        {
                            const readable<int> &mu_k = mu[k];
                            if(U(mu_k))
                            {
                                YACK_ASSERT(V(mu[k]));
                                break;
                            }
                        }

                    }
                }
            }
        }


        raven::qbranch source;
        source(mu,rank,accept_all);
        std::cerr << source << std::endl;





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

