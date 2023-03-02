

#include "yack/cameo/static-add.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"
#include "yack/system/rtti.hpp"

using namespace yack;

namespace {

#if 0
    template <typename T>
    static inline
    void perform_add( randomized::bits &ran )
    {
        cameo::add<T> xadd;
        list<T>   L;
        vector<T> V;
        for(size_t n=0;n<=10;++n)
        {
            std::cerr << "\tn=" << n << std::endl;
            xadd.free();
            L.free();
            V.free();
            T    sum = 0;
            for(size_t i=0;i<n;++i) {
                const T tmp = ran.choice() ? -bring::get<T>(ran) : bring::get<T>(ran);
                xadd.push(tmp);
                L    << tmp;
                V    << tmp;
                sum += tmp;
                //std::cerr << xadd << std::endl;
            }
            std::cerr << "\t\t" << sum << " / " << xadd.sum() << std::endl;
            std::cerr << "\t\t" << xadd.range(L) << " / " << xadd.range(V) << std::endl;
            std::cerr << "\t\t" << xadd.tableau(V) << " / " << xadd.tableau(V(),V.size()) << std::endl;

        }
    }
#endif


    template <typename T, const size_t N> static inline
    void perform_with(randomized::bits &ran)
    {
        typedef cameo::static_add<T,N> add_type;

        std::cerr << "\tN=" << N << " => size = " << sizeof(add_type) << std::endl;

        add_type  sadd;
        list<T>   L(N,as_capacity);
        vector<T> V(N,as_capacity);
        for(size_t cycle=1;cycle<=4;++cycle)
        {
            sadd.free();
            L.free();
            V.free();
            T    sum = 0;
            for(size_t i=0;i<N;++i) {
                const T tmp = ran.choice() ? -bring::get<T>(ran) : bring::get<T>(ran);
                sadd.push(tmp);
                L    << tmp;
                V    << tmp;
                sum += tmp;
            }
            std::cerr << "\t\t(*) " << sum             << " / " << sadd.sum()                 << std::endl;
            std::cerr << "\t\t    " << sadd.range(L)   << " / " << sadd.range(V)              << std::endl;
            std::cerr << "\t\t    " << sadd.tableau(V) << " / " << sadd.tableau(V(),V.size()) << std::endl;
        }

    }

    template <typename T>
    static inline void show_size(randomized::bits &ran)
    {
        std::cerr << "with <" << rtti::name<T>() << ">" << std::endl;
        perform_with<T,1>(ran);
        perform_with<T,2>(ran);
        perform_with<T,3>(ran);
        perform_with<T,4>(ran);
        perform_with<T,5>(ran);
        perform_with<T,6>(ran);
        perform_with<T,7>(ran);
        perform_with<T,8>(ran);

    }

}

YACK_UTEST(cameo_static_add)
{
    randomized::rand_ ran;
    show_size<float>(ran);
    show_size<double>(ran);
    show_size<long double>(ran);

}
YACK_UDONE()
