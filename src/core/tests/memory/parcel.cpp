
#include "yack/memory/parcel.hpp"
#include "yack/memory/allocator/pages.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(memory_parcel)
{
    memory::pages &mgr = memory::pages::instance();
    YACK_SIZEOF(memory::parcel);
    YACK_SIZEOF(memory::parcel::stamp_t);

    for(size_t block_exp2=memory::parcel::min_data_exp2;block_exp2<=10;++block_exp2)
    {
        void          *block_addr = mgr.query(block_exp2);
        const size_t   block_size = size_t(1) << block_exp2;
        {
            memory::parcel io(block_addr,block_size,block_exp2);

            std::cerr << "io="; io.display();
        }
        mgr.store(block_addr,block_exp2);
        
    }


}
YACK_UDONE()

