

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

    template <typename T>
    static inline void show_size()
    {
        std::cerr << "with <" << rtti::name<T>() << ">" << std::endl;
        std::cerr << sizeof(cameo::static_add<T,1>) << std::endl;
        std::cerr << sizeof(cameo::static_add<T,2>) << std::endl;
        std::cerr << sizeof(cameo::static_add<T,3>) << std::endl;
        std::cerr << sizeof(cameo::static_add<T,4>) << std::endl;
        std::cerr << sizeof(cameo::static_add<T,5>) << std::endl;
        std::cerr << sizeof(cameo::static_add<T,6>) << std::endl;
        std::cerr << sizeof(cameo::static_add<T,7>) << std::endl;
        std::cerr << sizeof(cameo::static_add<T,8>) << std::endl;


    }

}

YACK_UTEST(cameo_static_add)
{
    randomized::rand_ ran;
    show_size<float>();
    show_size<double>();
    show_size<long double>();




}
YACK_UDONE()
