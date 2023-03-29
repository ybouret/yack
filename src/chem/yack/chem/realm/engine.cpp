
#include "yack/chem/realm/engine.hpp"

namespace yack
{
    namespace chemical
    {
        engine:: ~engine() noexcept
        {
        }

        engine:: engine(const realm &cs) :
        engine_()
        {
            for(const domain *dom = cs.head; dom; dom=dom->next)
            {
                push_back( new reactor(*dom) );
            }
        }

    }

}


