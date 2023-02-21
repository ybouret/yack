#include "yack/apex/natural.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include <cmath>

namespace yack
{

    namespace apex
    {
        template <> float       natural::get_log2<float>()       noexcept { return log(2.0f); }
        template <> double      natural::get_log2<double>()      noexcept { return log(2.0);  }
        template <> long double natural::get_log2<long double>() noexcept { return log(2.0L); }


        void natural:: raise_log_error()
        {
            throw libc::exception(EDOM,"natural::log_of(0)");
        }
        
    }

}


