


#include "yack/memory/allocator/pages.hpp"
#include "yack/memory/note.hpp"
#include "yack/memory/book.hpp"

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

        static void *note__[ YACK_WORDS_FOR(note) ] = { 0 };
        static void *book__[ YACK_WORDS_FOR(book) ] = { 0 };

        pages:: pages()   : allocator(), singleton<pages>(),
        note_( new ( out_of_reach::zset(note__,sizeof(note__)) ) note() ),
        book_( new ( out_of_reach::zset(book__,sizeof(book__)) ) book() )
        {

        }

        pages:: ~pages() throw()
        {
            out_of_reach::zset( destructed(book_), sizeof(book__) );
            out_of_reach::zset( destructed(note_), sizeof(note__) );
            note_ = NULL;
            book_ = NULL;
        }

        void *pages:: acquire(size_t &count, const size_t block_size)
        {
            assert(block_size>0);

            YACK_LOCK(access);
            if(count>0)
            {
                size_t       page_size = count * block_size; if(page_size>book::max_page_size) throw libc::exception(ENOMEM,"%s max_page_size exceeded", variety() );
                const size_t page_exp2 = integer_log2( page_size = next_power_of_two(page_size) );
                try {
                    void *addr = query(page_exp2);
                    count      = page_size;
                    return addr;
                }
                catch(...)
                {
                    count=0; throw;
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
                assert(size<=book::max_page_size);
                store(addr,integer_log2(size));
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
            assert(page_exp2<=book::max_page_exp2);
            return (page_exp2<book::min_page_exp2) ? note_->query(page_exp2) : book_->query(page_exp2);
        }

        void  pages:: store(void *addr, const size_t page_exp2) throw()
        {
            YACK_LOCK(access);
            assert(page_exp2<=book::max_page_exp2);
            if(page_exp2<book::min_page_exp2)
                note_->store(addr,page_exp2);
            else
                book_->store(addr,page_exp2);
        }

    }
    
}

#include "yack/data/pool-sort.hpp"
#include "yack/memory/chunk.hpp"
#include <iostream>

namespace yack
{
    namespace memory
    {
        
        static inline page *chunk2page(chunk *ch) throw()
        {
            assert(NULL!=ch);
            return static_cast<page *>( out_of_reach::zset(ch,sizeof(page)));
        }
        
        void   pages:: book_store_pool(core_pool_of<chunk> &reservoir,
                                       const size_t         page_exp2) throw()
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert(page_exp2<=book::max_page_exp2);
            assert(page_exp2>=book::min_page_exp2);
            
            //__________________________________________________________________
            //
            // take two linked object to merge
            //__________________________________________________________________
            chapter           &lhs = (*book_)[page_exp2]; // get the right chapter
            pool_of<chunk>     rhs(reservoir);
            
            assert(lhs.memory_is_increasing());
            assert(rhs.memory_is_increasing());
            
            //__________________________________________________________________
            //
            // build the fusion list
            //__________________________________________________________________
            chapter fusion(lhs.page_size);
            
            while( (lhs.size>0) && (rhs.size>0) )
            {
                if ( (void*)lhs.head < (void*)rhs.head)
                {
                    fusion.push_back(lhs.pop_front());
                }
                else
                {
                    fusion.push_back( chunk2page(rhs.query()) );
                }
            }
            
            fusion.merge_back(lhs);
            while(rhs.size) fusion.push_back( chunk2page(rhs.query()) );
            
            //__________________________________________________________________
            //
            // and restore chapter
            //__________________________________________________________________
            lhs.swap_with(fusion);
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
            note_->display();
            book_->display();
            std::cerr << "<" << call_sign << "/>" << std::endl;

        }
    }
}
