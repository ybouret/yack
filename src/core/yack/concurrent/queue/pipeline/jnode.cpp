
#include "yack/concurrent/queue/pipeline/jnode.hpp"


namespace yack
{

    namespace concurrent
    {
        jnode:: jnode(const job_type &J,
                      const job_uuid  I) :
        next(0),
        prev(0),
        uuid(I),
        call(J)
        {
        }

        jnode:: ~jnode() throw()
        {
        }
        
    }

}
