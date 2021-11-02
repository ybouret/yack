#include "yack/apex/integer.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/exception.hpp"

namespace yack
{
    namespace apex
    {
        
        integer:: ~integer() throw()
        {
        }
        
        integer:: integer() : s(__zero__), n(0) {}
        
        integer:: integer(int_type z) :
        s( __sign::of(z) ),
        n( absolute(z)   )
        {
        }
        
        integer:: integer(const integer &z) :
        s(z.s),
        n(z.n)
        {
        }
        
        integer:: integer(const sign_type S, const natural &N) :
        s(S),
        n(N)
        {
            if(N<=0)
            {
                coerce(s) = __zero__;
            }
            else
            {
                if(s==__zero__)
                {
                    throw libc::exception(EINVAL,"%s(zero,n>0)",clid);
                }
            }
        }

        
        void integer:: xch(integer &z) throw()
        {
            coerce_cswap(s,z.s);
            coerce(n).xch( coerce(z.n) );
        }
        
        integer & integer:: operator=(const integer &z)
        {
            integer tmp(z);
            xch(tmp);
            return *this;
        }
        
        integer & integer:: operator=(const int_type z)
        {
            integer tmp(z);
            xch(tmp);
            return *this;
        }

        std::ostream & operator<<(std::ostream &os, const integer &z)
        {
            switch(z.s)
            {
                case negative: os << '-' << z.n; break;
                case positive: os << z.n; break;
                case __zero__: os << '0'; break;
            }
            return os;
        }
        
        const char integer::clid[] = "apz";
        
        const char *  integer::class_uid() const throw()
        {
            return clid;
        }
        
        
    }
}


namespace yack
{
    namespace apex
    {
        
        integer integer:: operator+() const
        {
            return *this;
        }
        
        integer integer:: operator-() const
        {
            return integer( __sign::opposite(s), n );
        }
        
    }
}

#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace apex
    {
        size_t integer::serialize(ios::ostream &os) const
        {
            const size_t nw = ios::encoder::emit<int8_t>(os,s);
            return nw + n.serialize(os);
        }
    }
    
}
