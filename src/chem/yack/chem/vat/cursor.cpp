#include "yack/chem/vat/cursor.hpp"

namespace yack
{
    namespace Chemical
    {

        Cursor:: Cursor(const double         c,
                        const Actor         &a,
                        const Species::Fund &f) :
        Limit(f)
        {
            Extended::Real &xi = *this;
            xi = Extended::Send(c) / a.xn;
            (*this) << *a;
        }


        Cursor:: ~Cursor() noexcept
        {
        }

    }

}
