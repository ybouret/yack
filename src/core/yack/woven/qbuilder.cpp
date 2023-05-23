
#include "yack/woven/qbuilder.hpp"

namespace yack
{
    namespace woven
    {
        qbuilder:: ~qbuilder() noexcept {}

        qbuilder:: qbuilder(const size_t dims) :
        metrics(dims),
        indices::fund( new indices::bank() ),
        parents(dims,*this),
        lineage(dims,*this)
        {
        }
        
    }

}


