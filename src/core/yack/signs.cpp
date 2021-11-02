
#include "yack/signs.hpp"

namespace yack
{
    
    const char * __sign::text(const sign_type s) throw()
    {
        switch(s)
        {
            case negative: return "negative";
            case __zero__: return "__zero__";
            case positive: return "positive";
        }
        return yack_unknown;
    }
    
    sign_type __sign:: opposite(const sign_type s) throw()
    {
        switch(s)
        {
            case negative: return positive;
            case positive: return negative;
            default:
                break;
        }
        return __zero__;
    }

    sign_pair __sign::pair(const sign_type lhs, const sign_type rhs) throw()
    {
        switch(lhs)
        {
            case negative:
                switch(rhs)
                {
                    case negative: return nn_pair;
                    case __zero__: return nz_pair;
                    case positive: return np_pair;
                }
            case __zero__:
                switch(rhs)
                {
                    case negative: return zn_pair;
                    case __zero__: return zz_pair;
                    case positive: return zp_pair;
                }
            case positive:
                switch(rhs)
                {
                    case negative: return pn_pair;
                    case __zero__: return pz_pair;
                    case positive: return pp_pair;
                }
        };
        
    }
    
    
    
}
