#include "yack/sequence/cxx-series.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/system/rtti.hpp"

using namespace yack;

namespace
{
    template <typename T> static inline
    void try_cxx_series(randomized::bits &ran)
    {
        std::cerr << std::endl;
        std::cerr << "cxx_series<" << rtti::name<T>() << ">" << std::endl;
        for(size_t cycle=0;cycle<10;++cycle)
        {
            cxx_series<T> s(1+ran.leq(10));
            std::cerr << "push_[back|front]" << std::endl;
            for(size_t iter=0;iter<1000;++iter)
            {
                s.free();
                while( s.size() < s.capacity() )
                {
                    const T args = bring::get<T>(ran);
                    if(ran.choice())
                        s.push_back(args);
                    else
                        s.push_front(args);
                }
            }

            std::cerr << "[add|pre] " << std::endl;
            for(size_t iter=0;iter<1000;++iter)
            {
                s.free();
                while( s.size() < s.capacity() )
                {
                    const T  d = bring::get<T>(ran);
                    const T &i = ran.choice() ? s.template pre<T>(d) : s. template add<T>(d);
                    YACK_ASSERT(d==i);
                }
            }


            std::cerr << "[ins]" << std::endl;
            for(size_t iter=0;iter<1000;++iter)
            {
                s.free();
                while( s.size() < s.capacity() )
                {
                    const T      d = bring::get<T>(ran);
                    const size_t i = ran.leq(s.size()+1);
                    const T     &j = s.template ins<T>(d,i);
                    YACK_ASSERT(d==j);
                }
            }

        }
    }

}

YACK_UTEST(data_cxx_series)
{
    randomized::rand_ ran;
    try_cxx_series<string>(ran);
    try_cxx_series<int>(ran);

}
YACK_UDONE()

