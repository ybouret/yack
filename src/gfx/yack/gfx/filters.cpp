
#include "yack/gfx/filters.hpp"


namespace yack
{
    namespace graphic
    {
        const char filters::Xext[] = "X";
        const char filters::Yext[] = "Y";

        filters:: filters(const filter &fx, const filter &fy) noexcept:
        X( & coerce(fx) ),
        Y( & coerce(fy) )
        {
        }


        filters:: filters(const filters &other) noexcept :
        X(other.X),
        Y(other.Y)
        {}

        filters:: ~filters() noexcept
        {
        }


    }

}
