#include "yack/math/fitting/least-squares/base.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            const char least_squares_:: clid[] = "[least-squares] ";

            least_squares_::  least_squares_() throw() {}
            least_squares_:: ~least_squares_() throw() {}

            const char * least_squares_:: ok(const bool flag)   throw()
            {
                return flag ? " <ok> " : " <no> ";
            }
        }

    }

}
