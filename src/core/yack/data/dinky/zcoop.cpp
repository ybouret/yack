#include "yack/data/dinky/zcoop.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace kernel
    {
       
        zcoop:: ~zcoop() throw()
        {
            assert(sync);
            delete sync;
            sync = 0;
        }
        
        zcoop:: zcoop() : object(), counted(),
        sync( new concurrent::mutex() )
        {
        }
        
        lockable & zcoop:: operator*() throw() {
            assert(sync);
            return *sync;
        }
    }
    
}
    
    
