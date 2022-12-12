#include "yack/apex/north/qmatrix.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/global.hpp"
#include "yack/memory/allocator/pooled.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;

namespace {


    static inline
    void fillv(writable<int> &tgt, randomized::bits &ran)
    {
        for(size_t i=tgt.size();i>0;--i) tgt[i] = static_cast<int>( ran.in(-10,10) );
    }

    template <typename T,typename ALLOCATOR> static inline
    void test_uniq(const size_t      dims,
                   randomized::bits &ran)
    {
        assert(dims>1);
        for(size_t cycle=0;cycle<16;++cycle)
        {
            north::qmatrix<T,ALLOCATOR> U(dims);
            vector<int> tmp(dims);
            while(U.situation!=north::almost_done)
            {
                fillv(tmp,ran);
                if(!U.grow(tmp)) continue;
            }
            std::cerr << "U=" << U << std::endl;
            vector<T> last(dims);

            {
                north::qmatrix<T,ALLOCATOR> V(U);
                while(V.situation!=north::fully_grown)
                {
                    fillv(tmp,ran);
                    if(!V.grow(tmp)) continue;;
                }
                for(size_t i=dims;i>0;--i) last[i] = V.last()[i];
            }
            std::cerr << "last=" << last << "[";

            for(size_t iter=0;iter<16;++iter)
            {
                north::qmatrix<T,ALLOCATOR> V(U);
                while(V.situation!=north::fully_grown)
                {
                    fillv(tmp,ran);
                    if(!V.grow(tmp)) continue;;
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
    
    YACK_SIZEOF(north::qvector<int64_t>);
    YACK_SIZEOF(north::qmatrix<int64_t>);


    YACK_SIZEOF(north::qvector<apq>);
    YACK_SIZEOF(north::qmatrix<apq>);


    for(size_t dims=2;dims<=20;++dims)
    {
        north::qmatrix<apq,memory::global> qg(dims);
        north::qmatrix<apq,memory::pooled> qp(dims);
        north::qmatrix<apq,memory::dyadic> qd(dims);

        north::qmatrix<int64_t,memory::global> qgi(dims);
        north::qmatrix<int64_t,memory::pooled> qpi(dims);
        north::qmatrix<int64_t,memory::dyadic> qdi(dims);


        std::cerr << "granted[" << std::setw(3) << dims << "] = @apq : "
        << std::setw(8) << qg.granted()
        << std::setw(8) << qp.granted()
        << std::setw(8) << qd.granted()
        << " | @int : "
        << std::setw(8) << qgi.granted()
        << std::setw(8) << qpi.granted()
        << std::setw(8) << qdi.granted()
        << std::endl;
    }

    {
        north:: qmatrix<apq,memory::global>     U(3);
        north:: qmatrix<int64_t,memory::global> V(3);


        matrix<int> vec(3,3);
        vec[1][1] = 1;  vec[1][2] = 2;  vec[1][3] = 3;
        vec[2][1] = 0;  vec[2][2] = 5;  vec[2][3] = 7;
        vec[3][1] = 11; vec[3][2] = 13; vec[3][3] = 0;

        for(size_t i=1;i<=vec.rows;++i)
        {
            YACK_CHECK(U.grow(vec[i])); std::cerr << U << std::endl;
            YACK_CHECK(V.grow(vec[i])); std::cerr << V << std::endl;
            YACK_CHECK(north::qmatrices::equality(V,U));
            YACK_CHECK(north::qmatrices::equality(U,V));
            YACK_CHECK(north::qmatrices::have_same_last(U,V));
            YACK_CHECK(north::qmatrices::have_same_last(V,U));
        }

        std::cerr << std::endl;
        std::cerr << U << std::endl;
        for(size_t iter=0;iter<=3;++iter)
        {
            U.shuffle(ran);
            std::cerr << U << std::endl;
            YACK_CHECK(north::qmatrices::equality(U,V));
            north:: qmatrix<apq,memory::global> Q(U);
            std::cerr << Q << std::endl;
            YACK_CHECK(north::qmatrices::equality(U,Q));
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
    for(size_t dims=2;dims<=8;++dims)
    {
        std::cerr << "-------- dims=" << dims << " --------" << std::endl;
        test_uniq<apq,memory::dyadic>(dims,ran);
        //test_uniq<int64_t,memory::pooled>(dims,ran);
        std::cerr << std::endl;
    }

}
YACK_UDONE()

