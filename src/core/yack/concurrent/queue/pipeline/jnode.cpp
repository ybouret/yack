
#include "yack/concurrent/queue/pipeline/jnode.hpp"


namespace yack
{

    namespace concurrent
    {
        jNode:: jNode(const job_type &J,
                      const job_uuid  I) :
        next(0),
        prev(0),
        uuid(I),
        call(J)
        {
        }

        jNode:: ~jNode() throw()
        {
        }
        
    }

}
