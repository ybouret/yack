#include "yack/sequence/vector.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sequence/list.hpp"

#include "yack/type/out-of-reach.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "../main.hpp"


using namespace yack;

template <typename SEQ>
static inline void test_seq( SEQ &seq )
{
    std::cerr << "mutable:" << std::endl;
    for(typename SEQ::iterator it=seq.begin();it!=seq.end();++it)
    {
        std::cerr << *it << "/";
    }
    std::cerr << std::endl;

    for(typename SEQ::reverse_iterator it=seq.rbegin();it!=seq.rend();++it)
    {
        std::cerr << *it << "/";
    }
    std::cerr << std::endl;

    std::cerr << "const" << std::endl;
    const SEQ &cst = seq;
    for(typename SEQ::const_iterator it=cst.begin();it!=cst.end();++it)
    {
        std::cerr << *it << "/";
    }
    std::cerr << std::endl;

    for(typename SEQ::const_reverse_iterator it=cst.rbegin();it!=cst.rend();++it)
    {
        std::cerr << *it << "/";
    }
    std::cerr << std::endl;


}

    


YACK_UTEST(data_iter)
{
    randomized::rand_ ran;

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
        std::cerr << "thin_array" << std::endl;
        thin_array<int> arr(data,5);
        test_seq(arr);
    }

    {
        std::cerr << "vector" << std::endl;
        vector<apq> Q;
        test_seq(Q);
        Q.push_back( bring::get<apq>(ran) );
        test_seq(Q);
        for(size_t i=1;i<=10+ran.leq(10);++i)
        {
            Q.push_back( bring::get<apq>(ran) );
        }
        test_seq(Q);
    }

    {
        std::cerr << "list" << std::endl;
        list<apn> L;
        test_seq(L);
        L.push_back( bring::get<apn>(ran) );
        test_seq(L);
        for(size_t i=1;i<=10+ran.leq(10);++i)
        {
            L.push_back( bring::get<apn>(ran) );
        }
        test_seq(L);
    }

    
}
YACK_UDONE()


