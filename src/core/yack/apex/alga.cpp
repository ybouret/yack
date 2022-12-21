
#include "yack/apex/alga.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{

    bool alga:: are_prop(const readable<int> &lhs, const readable<int> &rhs)
    {
        assert( lhs.size() == rhs.size() );
        auto_ptr<const apq> f = NULL;

        for(size_t i=lhs.size();i>0;--i)
        {
            const int l = lhs[i];
            const int r = rhs[i];
            switch( __sign::pair_of(l,r) )
            {
                case zz_pair: continue; // compatible;
                case zp_pair:
                case pz_pair:
                case zn_pair:
                case nz_pair:
                    return false; // incompatible

                case nn_pair:
                case pp_pair: {
                    const apq rho(absolute(l),absolute(r));
                    if(f.is_empty())
                    {
                        f = new apq(rho);
                    }
                    else
                    {
                        if( *f != rho ) return false;
                    }
                } continue;

                case np_pair:
                case pn_pair: {
                    const apq rho(absolute(l),absolute(r)); coerce(rho.num.s) = negative;
                    if(f.is_empty())
                    {
                        f = new apq(rho);
                    }
                    else
                    {
                        if( *f != rho ) return false;
                    }
                } continue;
            }
        }

        return true;
    }

}
