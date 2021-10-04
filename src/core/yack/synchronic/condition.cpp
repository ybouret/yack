#include "yack/synchronic/condition.hpp"
#include "yack/synchronic/quark/condition.hpp"

namespace yack
{
    namespace synchronic
    {
        condition:: condition() :
        impl( quark::condition_create() )
        {
            assert(impl);
        }
        
        
        condition:: ~condition() throw()
        {
            assert(NULL!=impl);
            quark::condition_delete(impl);
            impl=NULL;
        }
    }
    
}
