
#include "yack/memory/large/chapter.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{

    namespace memory
    {
        chapter:: chapter(const size_t sz) throw() :
        page_size(sz)
        {
            assert(page_size>=sizeof(page));
        }

        chapter:: ~chapter() throw()
        {
            while(size) page::release(pop_back(),page_size);
        }

        void  * chapter:: query()
        {
            if(size)
            {
                page *p = pop_front();
                return out_of_reach::zset(p,page_size);
            }
            else
            {
                return  page::acquire(page_size);
            }
        }

        void chapter:: store(void *addr) throw()
        {
            assert(addr);
            store_increasing_memory(static_cast<page*>(out_of_reach::zset(addr,sizeof(page))));
        }

    }

}

#include <iostream>
#include <iomanip>
#include "yack/arith/base2.hpp"
namespace yack
{

    namespace memory
    {

        void   chapter:: display() const
        {
            std::cerr << "    <chapter";
            const size_t page_exp2 = integer_log2(page_size);
            std::cerr << " page_size=\"2^" << std::setw(2) << page_exp2 << "\"";
            std::cerr << " available=\""   << std::setw(3) << size      << "\"";
            std::cerr << "/>" << std::endl;
        }

    }

}
