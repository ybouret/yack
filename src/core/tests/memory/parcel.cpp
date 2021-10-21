
#include "yack/memory/joint/parcel.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/data/cxx-list.hpp"
#include "yack/utest/run.hpp"
#include "yack/type/utils.hpp"

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
        YACK_DISABLE_ASSIGN(block);
        inline  block(const block &other) throw() : next(0), prev(0), addr(0), size(other.size) {}
    };
}

YACK_UTEST(memory_parcel)
{
    randomized::rand_  ran( time(NULL) );
    memory::dyadic &mgr = memory::dyadic::instance();
    YACK_SIZEOF(memory::parcel);
    YACK_SIZEOF(memory::parcel::stamp_t);

    for(size_t block_exp2=memory::parcel::min_data_exp2;block_exp2<=14;++block_exp2)
    {
        void              *block_addr = mgr.query(block_exp2);
        const size_t       block_size = size_t(1) << block_exp2;
        {
            memory::parcel     io(block_addr,block_size,block_exp2);
            cxx_list_of<block> st;
            io.display();

            for(size_t iter=0;iter<4;++iter)
            {
                while( true )
                {
                    size_t n = min_of<size_t>( ran.leq(100), ran.leq( io.capacity() ) );
                    void  *p = io.try_acquire(n);
                    if(!p)
                        break;
                    YACK_ASSERT(n>0);
                    st.push_back( new block(p,n) );
                }
                std::cerr << "acquired " << st.size << " blocks" << std::endl;
                std::cerr << "releasing..." << std::endl;
                io.display();
                randomized::shuffle::list(st,ran);
                while(st.size)
                {
                    block *b = st.pop_back();
                    YACK_ASSERT( &io == memory::parcel::get_release(b->addr,b->size) );
                    if(iter<=0) io.display();
                }
            }
            io.display();
            for(size_t i=1+ran.leq(7);i>0;--i)
            {
                size_t n = ran.leq( 5 * memory::parcel::stamp_size );
                (void)( io.try_acquire(n) );
            }

        }
        mgr.store(block_addr,block_exp2);
        std::cerr << std::endl;
    }


}
YACK_UDONE()

