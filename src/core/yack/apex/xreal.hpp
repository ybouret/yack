//! \file

#ifndef YACK_APEX_XREAL_INCLUDED
#define YACK_APEX_XREAL_INCLUDED 1

#include "yack/setup.hpp"
#include <iosfwd>

namespace yack
{
    namespace apex
    {
        //! value = m * 10^p
        template <typename T>
        class xreal
        {
        public:
            static const T   maximum;
            static const T   minimum;
            static T         ten_to(int q) noexcept;
            const T   m;
            const int p;

            xreal(const T     r);
            xreal(const xreal &) noexcept;
            ~xreal() noexcept;
            xreal & operator=(const xreal &) noexcept;

            friend std::ostream & operator<<(std::ostream &,const xreal &);
            
        };
    }
}

#endif

