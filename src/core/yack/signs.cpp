
#include "yack/signs.hpp"

namespace yack
{
    
    const char * __sign::text(const sign_type s) noexcept
    {
        switch(s)
        {
            case negative: return "negative";
            case __zero__: return "__zero__";
            case positive: return "positive";
        }
        return yack_unknown;
    }
    
    const char * __sign:: symbol(const sign_type s) noexcept
    {
        switch(s)
        {
            case negative: return "-";
            case __zero__: return "0";
            case positive: return "+";
        }
        return yack_unknown;
    }


    sign_type __sign:: opposite(const sign_type s) noexcept
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

    void __sign::change(sign_type &s) noexcept
    {
        s = opposite(s);
    }


    sign_pair __sign::pair(const sign_type lhs, const sign_type rhs) noexcept
    {
        switch(lhs)
        {
            case negative:
                switch(rhs)
                {
                    case negative: return nn_pair;
                    case __zero__: return nz_pair;
                    case positive: return np_pair;
                } break;

            case __zero__:
                switch(rhs)
                {
                    case negative: return zn_pair;
                    case __zero__: return zz_pair;
                    case positive: return zp_pair;
                } break;

            case positive:
                switch(rhs)
                {
                    case negative: return pn_pair;
                    case __zero__: return pz_pair;
                    case positive: return pp_pair;
                } break;
        }
        // never get here
        return zz_pair;
    }


    sign_type __sign:: product(const sign_type lhs, const sign_type rhs) noexcept
    {

        switch(lhs)
        {
            case negative:
                switch(rhs)
                {
                    case negative: return positive;
                    case __zero__: return __zero__;
                    case positive: return negative;
                } break;

            case __zero__:
                return __zero__;

            case positive:
                switch(rhs)
                {
                    case negative: return negative;
                    case __zero__: return __zero__;
                    case positive: return positive;
                } break;
        }
        // never get here
        return __zero__;
    }
    
    
}
