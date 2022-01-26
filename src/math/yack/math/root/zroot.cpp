#include "yack/math/root/zroot.hpp"


namespace yack
{

    namespace math
    {

        namespace core
        {

            zroot::  zroot() throw()
            {
            }

            zroot:: ~zroot() throw()
            {
            }

        }

    }

}

#define real_t float
#include "zroot.hxx"

#undef  real_t
#define real_t double
#include "zroot.hxx"


#undef  real_t
#define real_t long double
#include "zroot.hxx"
