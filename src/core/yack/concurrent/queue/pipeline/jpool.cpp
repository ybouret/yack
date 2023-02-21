
#include "yack/concurrent/queue/pipeline/jpool.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    namespace concurrent
    {
        jpool:: jpool() noexcept :  releasable(), impl(), uuid(1)
        {
        }

        void jpool:: release() noexcept
        {
            while(impl.size)
            {
                object::zrelease( impl.query() );
            }
        }

        jpool:: ~jpool() noexcept
        {
            release();
        }

        jnode * jpool:: zget()
        {
            return impl.size ? impl.query() : object::zacquire<jnode>();
        }

        void jpool:: zput(jnode *zombie) noexcept
        {
            assert(NULL!=zombie);
            impl.store( out_of_reach::naught(zombie) );
        }



        jnode   * jpool:: query(const job_type &J)
        {
            jnode *node = zget();
            try {   new (node) jnode(J,uuid); ++uuid; return node; }
            catch(...) { zput(node); throw;   }
        }

        void jpool:: store(jnode *alive) noexcept
        {
            assert(NULL!=alive);
            zput( destructed(alive) );
        }
    }

}
