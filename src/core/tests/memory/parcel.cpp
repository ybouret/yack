
#include "yack/memory/parcel.hpp"
#include "yack/memory/allocator/pages.hpp"
#include "yack/data/cxx-list.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class block
    {
    public:
        block *next;
        block *prev;
        void  *addr;
        size_t size;
        inline  block(void*p, size_t n) throw() : next(0), prev(0), addr(p), size(n) {}
        inline ~block() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };
}

YACK_UTEST(memory_parcel)
{
    uprng          ran;
    memory::pages &mgr = memory::pages::instance();
    YACK_SIZEOF(memory::parcel);
    YACK_SIZEOF(memory::parcel::stamp_t);

    for(size_t block_exp2=memory::parcel::min_data_exp2;block_exp2<=10;++block_exp2)
    {
        cxx_list_of<block> st;
        void              *block_addr = mgr.query(block_exp2);
        const size_t       block_size = size_t(1) << block_exp2;
        {
            memory::parcel io(block_addr,block_size,block_exp2);
            io.display();
            while( true )
            {
                size_t n = ran.leq( io.capacity() ) >> 2;
                void  *p = io.try_acquire(n);
                if(!p)
                    break;
                st.push_back( new block(p,n) );
            }
            std::cerr << "acquired " << st.size << " blocks" << std::endl;
            io.display();
        }
        mgr.store(block_addr,block_exp2);
        std::cerr << std::endl;
    }


}
YACK_UDONE()

