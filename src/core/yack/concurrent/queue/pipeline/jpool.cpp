
#include "yack/concurrent/queue/pipeline/jpool.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    namespace concurrent
    {
        jpool:: jpool() throw() :  releasable(), impl(), uuid(1)
        {
        }

        void jpool:: release() throw()
        {
            while(impl.size)
            {
                object::zrelease( impl.query() );
            }
        }

        jpool:: ~jpool() throw()
        {
            release();
        }

        jNode * jpool:: zget()
        {
            return impl.size ? impl.query() : object::zacquire<jNode>();
        }

        void jpool:: zput(jNode *zombie) throw()
        {
            assert(NULL!=zombie);
            impl.store( out_of_reach::naught(zombie) );
        }



        jNode   * jpool:: query(const job_type &J)
        {
            jNode *node = zget();
            try {   new (node) jNode(J,uuid); ++uuid; return node; }
            catch(...) { zput(node); throw;   }
        }

        void jpool:: store(jNode *alive) throw()
        {
            assert(NULL!=alive);
            zput( destructed(alive) );
        }
    }

}
