#include "yack/sequence/vector.hpp"
#include "yack/sequence/thin-array.hpp"

#include "yack/container/iterator/linked.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

template <typename SEQ>
static inline void test_seq( SEQ &seq )
{
#if 1
    for(typename SEQ::iterator it=seq.begin();it!=seq.end();it++)
    {
        std::cerr << *it << "/";
    }
#endif
}

    


YACK_UTEST(data_iter)
{
    std::cerr << iterating::direction_text(iterating::forward) << std::endl;
    std::cerr << iterating::direction_text(iterating::reverse) << std::endl;
    
    
    
    int data[] = { 1, 2, 3, 4, 5 };
    {
        {
            const iterating::linear<int,iterating::forward> end( out_of_reach::haul(data,+5) );
            iterating::linear<int,iterating::forward>       cur(data);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }
        
        {
            const iterating::linear<const int,iterating::forward> end(out_of_reach::haul(data,+5));
            iterating::linear<const int,iterating::forward>       cur(data+0);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }
        
        {
            const iterating::linear<int,iterating::reverse> end(out_of_reach::prev(data));
            iterating::linear<int,iterating::reverse>       cur(data+4);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }
        
        {
            const iterating::linear<const int,iterating::reverse> end(out_of_reach::prev(data));
            iterating::linear<const int,iterating::reverse>       cur(data+4);
            while(cur!=end)
            {
                std::cerr << *(cur++) << std::endl;
            }
        }
        
    }
    
    {
        thin_array<int> arr(data,5);
        test_seq(arr);
    }
    
    
    
}
YACK_UDONE()


