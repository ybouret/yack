
#include "yack/apex/integer.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace apex
    {
        integer integer:: div(const handle &lh, const handle &rh)
        {
            switch( __sign::pair(lh.s,rh.s) )
            {
                case nz_pair:
                case zz_pair:
                case pz_pair:
                    throw libc::exception(EDOM,"integer division by zero");

                case zn_pair:
                case zp_pair:
                    return integer();

                case nn_pair:
                case pp_pair:
                {
                    const  natural n = natural::div(lh,rh);
                    return integer(positive,n);
                }

                case np_pair:
                case pn_pair:
                {
                    const  natural n = natural::div(lh,rh);
                    return integer(negative,n);
                }

            }
            throw exception("integer::div(CORRUPTED)");
        }
    }
}
