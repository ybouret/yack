#include "yack/apex/integer.hpp"
#include "yack/system/error.hpp"

namespace yack
{
    namespace apex
    {
        int integer::cmp(const handle &lh, const handle &rh) throw()
        {
            switch( __sign::pair(lh.s,rh.s) )
            {
                case nn_pair: return natural::cmp(rh,lh);
                case nz_pair: return -1;
                case np_pair: return -1;

                case zn_pair: return  1;
                case zz_pair: return  0;
                case zp_pair: return -1;

                case pn_pair: return 1;
                case pz_pair: return 1;
                case pp_pair: return natural::cmp(lh,rh);
            }

            system_error::never_get_here("integer::cmp");
            return 0;
        }

        sign_type integer::scmp(const handle &lh, const handle &rh) throw()
        {
            switch( __sign::pair(lh.s,rh.s) )
            {
                case nn_pair: return natural::scmp(rh,lh);
                case nz_pair: return negative;
                case np_pair: return negative;

                case zn_pair: return positive;
                case zz_pair: return __zero__;
                case zp_pair: return negative;

                case pn_pair: return positive;
                case pz_pair: return positive;
                case pp_pair: return natural::scmp(lh,rh);
            }
            system_error::never_get_here("integer::scmp");
            return __zero__;
        }

    }

}
