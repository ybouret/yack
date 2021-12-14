
#include "yack/randomized/rand.hpp"
#include "yack/randomized/park-miller.hpp"
#include "yack/randomized/shuffle.hpp"

#include "yack/data/list/raw.hpp"
#include "yack/data/pool/raw.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/seed.hpp"


#include <cstring>

using namespace yack;

namespace
{
    template <typename T>
    struct node
    {
        node *next;
        node *prev;
        T     data;

        friend inline std::ostream & operator<<(std::ostream &os, const node &n)
        {
            os << n.data;
            return os;
        }
    };
}

YACK_UTEST(rand_shuffle)
{
    static const size_t N = 30;
    randomized::rand_   ran;
    
    {
        uint64_t words[N];
        for(size_t i=0;i<N;++i) words[i] = i;
        randomized::shuffle::data(words,N,ran);
        for(size_t i=0;i<N;++i) std::cerr << ' ' << words[i];
        std::cerr << std::endl;
    }


    {
        typedef node<int> node_t;
        node_t nodes[N];
        memset(nodes,0,sizeof(nodes));
        raw_list_of<node_t> l;
        for(size_t i=0;i<N;++i)
        {
            l.push_back(nodes+i)->data = int(i);
        }
        randomized::shuffle::list(l,ran);
        for(const node_t *p=l.head;p;p=p->next) std::cerr << ' ' << *p;
        std::cerr << std::endl;
        l.restart();
    }


    {
        typedef node<int> node_t;
        node_t nodes[N];
        memset(nodes,0,sizeof(nodes));
        raw_pool_of<node_t> l;
        for(size_t i=0;i<N;++i)
        {
            l.store(nodes+i)->data = int(i);
        }
        randomized::shuffle::pool(l,ran);
        for(const node_t *p=l.head;p;p=p->next) std::cerr << ' ' << *p;
        std::cerr << std::endl;
        l.restart();
    }




}
YACK_UDONE()
