
#include "yack/concurrent/queue/jpool.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    namespace concurrent
    {
        jPool:: jPool() throw() :  releasable(), impl(), uuid(1)
        {
        }

        void jPool:: release() throw()
        {
            while(impl.size)
            {
                object::zrelease( impl.query() );
            }
        }

        jPool:: ~jPool() throw()
        {
            release();
        }

        jNode * jPool:: zget()
        {
            return impl.size ? impl.query() : object::zacquire<jNode>();
        }

        void jPool:: zput(jNode *zombie) throw()
        {
            assert(NULL!=zombie);
            impl.store( out_of_reach::naught(zombie) );
        }



        jNode   * jPool:: query(const job_type &J)
        {
            jNode *node = zget();
            try {   new (node) jNode(J,uuid); ++uuid; return node; }
            catch(...) { zput(node); throw;   }
        }

        void jPool:: store(jNode *alive) throw()
        {
            assert(NULL!=alive);
            zput( destructed(alive) );
        }
    }

}
