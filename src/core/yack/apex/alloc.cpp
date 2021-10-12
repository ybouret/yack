#include "yack/apex/alloc.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{

    namespace apex
    {

        void * alloc:: block_acquire(int &block_exp2)
        {
            static memory::dyadic &mgr = memory::dyadic::instance();
            assert(block_exp2>=0);
            try {
                return mgr.query( unsigned(block_exp2) );
            }
            catch(...)
            {
                block_exp2=-1;
                throw;
            }
        }

        void  alloc:: block_release(void * &block_addr, int &block_exp2) throw()
        {
            static memory::dyadic &mgr = memory::dyadic::location();
            assert(NULL!=block_addr);
            assert(block_exp2>=0);
            mgr.store(block_addr,unsigned(block_exp2));
            block_addr = NULL;
            block_exp2 = -1;
        }


    }

}
