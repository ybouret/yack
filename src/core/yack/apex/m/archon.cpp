
#include "yack/apex/m/archon.hpp"
#include "yack/apex/m/hoard.hpp"
#include "yack/check/static.hpp"

namespace yack
{

    namespace apex
    {

        const char archon::call_sign[] = "apex::archon";

        static void *hoard__[ YACK_WORDS_FOR(hoard) ];

        archon:: ~archon() throw()
        {
        }

        archon:: archon() throw() :
        singleton<archon>(),
        cache( new ( out_of_reach::zset(hoard__,sizeof(hoard__)) ) hoard() )
        {

        }

        void * archon:: acquire(size_t &block_exp2)
        {
            YACK_LOCK(access);
            return cache->acquire_unlocked(block_exp2);
        }

        void archon::release(void *block_addr, size_t block_exp2) throw()
        {
            YACK_LOCK(access);
            cache->release_unlocked(block_addr,block_exp2);
        }


    }
}
