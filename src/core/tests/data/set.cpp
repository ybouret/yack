#include "yack/data/set.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace
{
    static inline void torture( data_set<int> &ids, randomized::bits &ran)
    {
        ids.free();
        size_t ops   = 0;
        size_t count = 0;
        while(true)
        {
            const int value = static_cast<int>( ran.in(1,100) );
            if( ran.choice() )
            {
                if(ids.insert(value))
                {
                    ++ops;
                    std::cerr << '+';
                }
                else
                    std::cerr << '.';
            }
            else
            {
                if(ids.remove(value))
                {
                    ++ops;
                    std::cerr << '-';
                }
                else
                    std::cerr << '.';
            }
            if(++count>=64)
            {
                std::cerr << std::endl;
                count = 0;
            }

            for(const data_set<int>::node_type *node=ids->head;node;node=node->next)
            {
                if(node->next)
                {
                    YACK_ASSERT(**node<**(node->next));
                }
            }

            if(ops>=10000) break;
        }
        std::cerr << std::endl;

        std::cerr << *ids << std::endl;

        YACK_ASSERT(ids.includes(ids));

        for(size_t iter=0;iter<10;++iter)
        {
            data_set<int> sub( ids->cache );
            for(const data_set<int>::node_type *node=ids->head;node;node=node->next)
            {
                if( ran.to<double>() < 0.4 ) sub.ensure( **node );
            }
            YACK_CHECK(ids.includes(sub));

            data_set<int> tmp = ids;
            YACK_CHECK(tmp==ids);
            tmp += sub;
            YACK_CHECK(tmp==ids);

        }


    }
}


YACK_UTEST(data_set)
{
    randomized::rand_ ran;

    data_set<int>::fund_type izp = new data_set<int>::bank_type();
    izp->reserve(100);

    data_set<int> ids( izp );

    std::cerr << "Inserting On Empty" << std::endl;
    YACK_ASSERT(ids.insert(10));
    YACK_ASSERT(ids.contains(10));
    YACK_ASSERT(!ids.insert(10));
    YACK_ASSERT(!ids.contains(1));
    YACK_ASSERT(!ids.contains(100));
    std::cerr << std::endl;

    std::cerr << "Inserting On Size=1, Lower" << std::endl;
    ids.free();
    YACK_ASSERT(ids.insert(10)); std::cerr << *ids << std::endl;
    YACK_ASSERT(ids.insert(1));  std::cerr << *ids << std::endl;
    YACK_ASSERT(ids.contains(10));
    YACK_ASSERT(ids.contains(1));
    YACK_ASSERT(!ids.contains(0));
    YACK_ASSERT(!ids.contains(11));
    YACK_ASSERT(!ids.contains(5));

    std::cerr << "Inserting On Size=1, Upper" << std::endl;
    ids.free();
    YACK_ASSERT(ids.insert(1));
    YACK_ASSERT(ids.insert(10));
    YACK_ASSERT(ids.contains(10));
    YACK_ASSERT(ids.contains(1));
    YACK_ASSERT(!ids.contains(0));
    YACK_ASSERT(!ids.contains(11));
    YACK_ASSERT(!ids.contains(5));
    std::cerr << std::endl;

    std::cerr << "Growing " << *ids << std::endl;
    YACK_ASSERT(ids.insert(100));
    YACK_ASSERT(ids.contains(1));
    YACK_ASSERT(ids.contains(10));
    YACK_ASSERT(ids.contains(100));

    YACK_ASSERT(!ids.contains(0));
    YACK_ASSERT(!ids.contains(5));
    YACK_ASSERT(!ids.contains(50));
    YACK_ASSERT(!ids.contains(150));
    std::cerr << std::endl;

    ids.free();
    YACK_ASSERT(ids.insert(10));
    YACK_ASSERT(ids.insert(100));
    std::cerr << "Growing " << *ids << std::endl;
    YACK_ASSERT(ids.insert(1));

    ids.free();
    YACK_ASSERT(ids.insert(10));
    YACK_ASSERT(ids.insert(100));
    std::cerr << "Growing " << *ids << std::endl;
    YACK_ASSERT(ids.insert(50));
    std::cerr << std::endl;

    YACK_ASSERT(ids.remove(10));
    YACK_ASSERT(ids.remove(50));
    YACK_ASSERT(ids.remove(100));

    torture(ids,ran);


    
}
YACK_UDONE()

