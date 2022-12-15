#include "yack/raven/qmatrix.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/utest/run.hpp"

#include "yack/sequence/vector.hpp"
#include "yack/container/matrix.hpp"
#include "yack/ios/ascii/convert.hpp"

#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"


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
            YACK_ASSERT(U(nu[i]));
        }
        std::cerr << "U=" << U << std::endl;

        for(size_t i=1;i<=rank;++i)
        {
            YACK_ASSERT(U.includes(nu[i]));
        }

        raven::qmatrix V(size,rank);

        
        const size_t n = rank;
        vector<apz>  u(size);
        vector<apz>  v(size);

        for(size_t k=2;k<n;++k)
        {
            combination    comb(n,k);
            vector<size_t> indx(k);
            vector<size_t> left(n-k);
            do
            {
                for(size_t i=1,j=1;i<=n;++i)
                {
                    if(comb.look_up(i)) continue;
                    left[j++] = i;
                }

                // create subspace in U
                U.reset();
                std::cerr << "using " << comb << " | left=" << left << std::endl;
                for(size_t i=1;i<=k;++i)
                {
                    YACK_ASSERT(U(nu[comb[i]]));
                }

                for(size_t i=1;i<=k;++i)
                {
                    YACK_ASSERT(U.includes(nu[comb[i]]));
                }

                for(size_t i=left.size();i>0;--i)
                {
                    YACK_ASSERT(!U.includes(nu[left[i]]));
                }


                // create shuffled subspace in V
                permutation perm(k);
                do
                {
                    if(1==perm.index) continue;
                    perm.designate(indx,comb);
                    V.reset();
                    for(size_t i=1;i<=k;++i)
                    {
                        YACK_ASSERT(V(nu[indx[i]]));
                    }
                    for(size_t i=1;i<=k;++i)
                    {
                        YACK_ASSERT(V.includes(nu[comb[i]]));
                    }
                    for(size_t i=left.size();i>0;--i)
                    {
                        YACK_ASSERT(!V.includes(nu[left[i]]));
                    }

                    for(size_t i=left.size();i>0;--i)
                    {
                        const readable<int> &cf = nu[ left[i] ];
                        YACK_ASSERT(U.guess(u,cf));
                        YACK_ASSERT(V.guess(v,cf));
                        YACK_ASSERT(u==v);
                    }



                } while(perm.next());


            } while( comb.next() );
        }


    }


}

YACK_UTEST(raven_space)
{
    randomized::rand_ ran;

    size_t size = 7; if(argc>1) size = ios::ascii::convert::to<size_t>(argv[1]);
    size_t rank = 4; if(argc>2) rank = ios::ascii::convert::to<size_t>(argv[2]);


    test(size,rank,ran);


    YACK_SIZEOF(raven::qvector);
    YACK_SIZEOF(raven::qmatrix);


}
YACK_UDONE()

