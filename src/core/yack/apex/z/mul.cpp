
#include "yack/apex/integer.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace apex
    {
        integer integer::mul(const handle &lh, const handle &rh)
        {
            switch( __sign::pair(lh.s,rh.s) )
            {
                case zn_pair:
                case zz_pair:
                case zp_pair:
                case nz_pair:
                case pz_pair:
                    return integer();

                case nn_pair:
                case pp_pair: {
                    const  natural n = natural::mul(lh,rh);
                    return integer(positive,n);
                }

                case np_pair:
                case pn_pair: {
                    const  natural n = natural::mul(lh,rh);
                    return integer(negative,n);
                }

            }
            throw exception("integer::mul(CORRUPTED)");
        }

    }

}
