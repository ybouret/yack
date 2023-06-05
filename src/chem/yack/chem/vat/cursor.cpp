#include "yack/chem/vat/cursor.hpp"

namespace yack
{
    namespace Chemical
    {

        Cursor:: Cursor(const Extended::Real &c,
                        const Actor          &a,
                        const Species::Fund  &f) :
        Limit(f)
        {
            Extended::Real &xi = *this;
            xi = c / a.xn;
            (*this) << *a;
        }


        Cursor:: ~Cursor() noexcept
        {
        }

    }

}
