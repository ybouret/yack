
#include "yack/woven/metrics.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace woven
    {
        metrics:: metrics(const size_t dims) :
        dimensions(dims)
        {
            if(dimensions<=1) throw libc::exception(EDOM,"invalid woven::metrics::dimensions=%u", unsigned(dims));
        }

        metrics:: ~metrics() noexcept {}

        metrics:: metrics(const metrics &other) noexcept : dimensions(other.dimensions) {}
        
    }

}

