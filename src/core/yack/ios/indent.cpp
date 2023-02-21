
#include "yack/ios/indent.hpp"

namespace yack
{
    namespace ios
    {
        indent:: ~indent() noexcept
        {
        }

        indent:: indent(const size_t sz, const char sp) noexcept :
        n(sz),
        c(sp)
        {
        }

        indent:: indent(const indent &_) noexcept : n(_.n), c(_.c) {}
        
    }

}
