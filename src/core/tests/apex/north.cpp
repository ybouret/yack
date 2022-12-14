#include "yack/apex/north/qmatrix.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ascii/convert.hpp"

using namespace yack;

namespace {


    static inline
    void fillv(writable<int> &tgt, randomized::bits &ran)
    {
        for(size_t i=tgt.size();i>0;--i) tgt[i] = static_cast<int>( ran.in(-10,10) );
    }

    // testing that the last is unique
    void test_uniq(const size_t      dims,
                   randomized::bits &ran)
    {
        assert(dims>1);
        for(size_t cycle=0;cycle<16;++cycle)
        {
            north::qmatrix  U(dims);
            vector<int> tmp(dims);

            // create a qbase up to almost_done
            while(U.situation!=north::almost_done)
            {
                fillv(tmp,ran);
                if(!U.grow(tmp)) continue;
            }
            std::cerr << "U=" << U << std::endl;

            // compute last vector from a copy a U
            vector<apq> last(dims);
            {
                north::qmatrix  V(U);
                while(V.situation!=north::fully_grown)
                {
                    fillv(tmp,ran);
                    if(!V.grow(tmp)) continue;;
                }
                for(size_t i=dims;i>0;--i) last[i] = V.last()[i];
            }
            std::cerr << "last=" << last << "[";

            // compute other last from a copy of U
            for(size_t iter=0;iter<16;++iter)
            {
                north::qmatrix  V(U);
                while(V.situation!=north::fully_grown)
                {
                    fillv(tmp,ran);
                    if(!V.grow(tmp)) continue;
                }

                if( comparison::disparity(last,V.last())) throw exception("failure!!");
                std::cerr << ".";
            }
            std::cerr << "]" << std::endl;
        }


    }

}

YACK_UTEST(apex_north)
{
    randomized::rand_ ran;
    
    
    
    
    {
        north:: qmatrix U(3);
        north:: qmatrix V(3);
        
        
        matrix<int> vec(3,3);
        vec[1][1] = 1;  vec[1][2] = 2;  vec[1][3] = 3;
        vec[2][1] = 0;  vec[2][2] = 5;  vec[2][3] = 7;
        vec[3][1] = 11; vec[3][2] = 13; vec[3][3] = 0;
        
        for(size_t i=1;i<=vec.rows;++i)
        {
            YACK_CHECK(U.grow(vec[i])); std::cerr << U << std::endl;
            YACK_CHECK(V.grow(vec[i])); std::cerr << V << std::endl;
            YACK_CHECK(north::qmatrix::equality(V,U));
            YACK_CHECK(north::qmatrix::equality(U,V));
        }

        
        std::cerr << std::endl;
        std::cerr << U << std::endl;
        for(size_t iter=0;iter<=3;++iter)
        {
            U.shuffle(ran);
            std::cerr << U << std::endl;
            YACK_CHECK(north::qmatrix::equality(U,V));
            north:: qmatrix  Q(U);
            std::cerr << Q << std::endl;
            YACK_CHECK(north::qmatrix::equality(U,Q));
        }
        
        for(size_t iter=0;iter<10;++iter)
        {
            writable<int> &any = vec[1];
            fillv(any,ran);
            YACK_CHECK(!U.grow(any));
            YACK_CHECK(!V.grow(any));
        }
        
    }
    
    std::cerr << std::endl << "-- univocal" << std::endl;
    size_t maxDims = 8;
    if(argc>1)
    {
        maxDims = ios::ascii::convert::to<size_t>(argv[1]);
    }
        
    for(size_t dims=2;dims<=maxDims;++dims)
    {
        std::cerr << "-------- dims=" << dims << " --------" << std::endl;
        test_uniq (dims,ran);
        std::cerr << std::endl;
    }
    
    YACK_SIZEOF(north::qmatrix);
    YACK_SIZEOF(north::qvector);
    YACK_SIZEOF(apn);

}
YACK_UDONE()

