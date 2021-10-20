#include "yack/apex/m/hoard.hpp"
#include "yack/type/utils.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <iomanip>

namespace yack
{

    namespace apex
    {

        void  hoard:: display() const
        {
            std::cerr << "<apex::hoard>" << std::endl;
            size_t active = 0;
            size_t blocks = 0;
            for(size_t i=min_block_exp2;i<=max_block_exp2;++i)
            {
                const repository &r = repo[i];
                if(r.size)
                {
                    ++active;
                    blocks += r.size;
                    std::cerr << "  <repo '2^" << std::setw(2) << i << "' #=" << std::setw(6) << r.size << " />" << std::endl;
                }
            }
            std::cerr << "  #block=" << blocks << " in #active=" << active << std::endl;
            std::cerr << "<apex::hoard/>" << std::endl;
        }

        void  hoard:: gc() throw()
        {
            for(size_t i=min_block_exp2;i<=max_block_exp2;++i)
            {
                repository &r = repo[i];
                if(r.size>0)
                {
                    static memory::dyadic &mgr = memory::dyadic::instance();
                    while(r.size)
                    {
                        mgr.store(r.pop(),i);
                    }
                }
                assert( out_of_reach::is0(&r,sizeof(repository)) );
            }
        }


        hoard:: ~hoard() throw()
        {
            gc();
            repo = 0;
        }

        hoard:: hoard() throw() : repo(0), impl()
        {
            repo = static_cast<repository*>( out_of_reach::zset(impl,sizeof(impl)) )-min_block_exp2;
        }


        void * hoard:: acquire_unlocked(size_t &block_exp2)
        {
            if(block_exp2>max_block_exp2) throw libc::exception(ENOMEM,"memory limit exceede in apex");
            if(block_exp2<min_block_exp2) block_exp2 = min_block_exp2;

            repository &r = repo[block_exp2];
            if(r.size)
            {
                return r.pop();
            }
            else
            {
                static memory::dyadic &mgr = memory::dyadic::instance();
                try {
                    return mgr.query(block_exp2);
                }
                catch(...)
                {
                    block_exp2=0;
                    throw;
                }
            }
            
        }

        void hoard:: release_unlocked(void *block_addr, const size_t block_exp2) throw()
        {
            assert(NULL!=block_addr);
            assert(block_exp2>=min_block_exp2);
            assert(block_exp2<=max_block_exp2);
            memset(block_addr,0,sizeof(piece));
            repo[block_exp2].push( static_cast<piece *>(block_addr) );
        }

        

    }

}
