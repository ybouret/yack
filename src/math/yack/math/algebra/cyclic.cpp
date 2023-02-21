
#include "yack/math/algebra/cyclic.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack {

    namespace math {

        cyclic_:: ~cyclic_() noexcept
        {
        }

        cyclic_:: cyclic_(const size_t n)
        {
            if(n<3) throw libc::exception(EDOM,"cyclic system is too small");
        }
    }

}


