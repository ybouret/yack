
#include "yack/ios/indent.hpp"

namespace yack
{
    namespace ios
    {
        indent:: ~indent() throw()
        {
        }

        indent:: indent(const size_t sz, const char sp) throw() :
        n(sz),
        c(sp)
        {
        }

        indent:: indent(const indent &_) throw() : n(_.n), c(_.c) {}
        
    }

}
