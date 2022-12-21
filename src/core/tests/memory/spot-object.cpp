#include "yack/spot-object.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/arith/align.hpp"
#include <cstring>

using namespace yack;

namespace {
    
    static size_t all_blocks = 0;
    
    template <size_t N>
    class block : public spot_object
    {
    public:
        block *next;
        block *prev;
        void  *data[N];
        
        block() throw() : next(0), prev(0), data()
        {
            memset(data,0,sizeof(data));
            ++all_blocks;
        }
        
        ~block() throw()
        {
            --all_blocks;
        }
        
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };
    
    
    
    
    template <size_t N> static inline
    void do_test_obj(randomized::bits &ran)
    {
        typedef block<N> Block;
        std::cerr << "N=" << N << ": ";YACK_SIZEOF(block<N>);
        {
            cxx_list_of<Block> l;
            for(size_t i=1000+ran.leq(1000);i>0;--i)
            {
                l.push_back(new Block());
            }
            
            randomized::shuffle::list(l,ran);
        }
        YACK_CHECK("new/delete" && all_blocks<=0);
        for(size_t i=0;i<=1000;++i)
        {
            Block *B = new Block[i];
            delete []B;
        }
        YACK_CHECK("new[]/delete[]" && all_blocks<=0);
        
        void *impl[ YACK_WORDS_FOR(Block) ];
        ran.fill(impl,sizeof(impl));
        Block *b = new (impl) Block();
        b->~Block();
        YACK_CHECK("placement" && all_blocks<=0);
        
        
    }
    
    
}

YACK_UTEST(spot_object)
{
    randomized::rand_  ran;
    do_test_obj<1>(ran);
    do_test_obj<5>(ran);
    do_test_obj<13>(ran);
    do_test_obj<29>(ran);

}
YACK_UDONE()
