#include "yack/aqueous/realm/collector.hpp"

namespace yack
{
    namespace aqueous
    {

        
        collector:: ~collector() noexcept
        {

        }

        collector:: collector(const size_t n) :
        collecting::caches(),
        collector_(n,*this)
        {
        }

    }

}


