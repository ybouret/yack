

#include "yack/memory/allocator/pages.hpp"
#include "yack/arith/base2.hpp"
#include "yack/system/exception.hpp"
#include "yack/type/utils.hpp"
#include <new>
#include <cerrno>

namespace yack
{
    namespace memory
    {

        const size_t pages::max_page_size = book::max_page_size;
        const size_t pages::min_page_size = book::min_page_size;


        static void *book__[ YACK_WORDS_FOR(book) ] = { 0 };

        pages:: pages() throw() : allocator(), singleton<pages>(),
        tome( new ( out_of_reach::zset(book__,sizeof(book__)) ) book() )
        {

        }

        pages:: ~pages() throw()
        {
            out_of_reach::zset( destructed(tome), sizeof(book__) );
            tome = NULL;
        }

        void *pages:: acquire(size_t &count, const size_t block_size)
        {
            assert(block_size>0);

            YACK_LOCK(access);
            if(count>0)
            {
                // check page_size
                size_t page_size = max_of(count * block_size,book::min_page_size);
                if(page_size>book::max_page_size) throw libc::exception(ENOMEM,"%s max_page_size exceeded", variety() );

                // find and query page
                try
                {
                    void *addr = tome->query( integer_log2( page_size = next_power_of_two(page_size) ) );
                    count = page_size; assert( is_a_power_of_two(count) );
                    return addr;
                }
                catch(...)
                {
                    count=0;
                    throw;
                }
            }
            else
            {
                return NULL;
            }
        }

        void pages:: release(void *&addr, size_t &size) throw()
        {
            YACK_LOCK(access);
            if(addr)
            {
                assert(size>0);
                assert(is_a_power_of_two(size));
                assert(size>=book::min_page_size);
                assert(size<=book::max_page_size);
                tome->store(addr,integer_log2(size));
                addr = 0;
                size = 0;
            }
            else
            {
                assert(size<=0);
            }
        }

        const char   pages:: call_sign[] = "memory::pages";

        const char * pages:: variety() const throw() { return call_sign; }

        void * pages::query(const size_t page_exp2)
        {
            YACK_LOCK(access);
            return tome->query(page_exp2);
        }

        void  pages:: store(void *addr, const size_t page_exp2) throw()
        {
            YACK_LOCK(access);
            tome->store(addr,page_exp2);
        }

        
    }

}

#include <iostream>
namespace yack
{
    namespace memory
    {
        void  pages:: display() const
        {
            YACK_LOCK(access);
            std::cerr << "<" << call_sign << ">" << std::endl;
            tome->display();
            std::cerr << "<" << call_sign << "/>" << std::endl;

        }
    }
}
