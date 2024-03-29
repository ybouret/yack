#include "yack/apex/integer.hpp"
#include "yack/type/utils.hpp"
#include "yack/type/abs.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#include "yack/randomized/bits.hpp"

namespace yack
{
    namespace apex
    {
        
        integer:: ~integer() noexcept
        {
        }
        
        integer:: integer() : number(), s(__zero__), n(0) {}

        integer integer:: abs() const
        {
            if(negative==s)
            {
                return integer(positive,n);
            }
            else
            {
                return *this;
            }
        }

        integer integer:: mod2() const
        {
            if(__zero__!=s)
            {
                const  natural tmp = natural::squared(n);
                return integer(tmp);
            }
            else
            {
                return *this;
            }

        }
        


        integer:: integer(int_type z) :
        number(),
        s( __sign::of(z) ),
        n( absolute(z)   )
        {
        }
        
        integer:: integer(const integer &z) : number(),
        s(z.s),
        n(z.n)
        {
        }
        
        integer:: integer(const sign_type S, const natural &N) :
        number(),
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

        integer:: integer(const natural &N) :
        s( (N>0) ? positive : __zero__ ),
        n( N )
        {
        }

        integer:: integer(randomized::bits &ran, const size_t nbits) :
        s(nbits<=0?__zero__ : (ran.choice()?positive:negative)),
        n(ran,nbits)
        {
            
        }

        void integer:: ldz() noexcept
        {
            coerce(s) = __zero__;
            coerce(n).ldz();
        }

        void integer:: ldi(int_type i) noexcept
        {
            switch( coerce(s) = __sign::of(i) )
            {
                case __zero__: coerce(n).ldz(); return;
                case positive: coerce(n).ldu( static_cast<uint_type>(i)  ); return;
                case negative: coerce(n).ldu( static_cast<uint_type>(-i) ); return;
            }
        }


        void integer:: xch(integer &z) noexcept
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
        
        const char *  integer::class_uid() const noexcept
        {
            return clid;
        }

        void integer:: simplify(integer &num, integer &den)
        {
            natural::simplify( coerce(num.n), coerce(den.n) );
        }
        
    }
}



namespace yack
{

    namespace apex
    {
        integer::handle_::  handle_() noexcept : u(0) {}
        integer::handle_:: ~handle_() noexcept {}

        integer::handle_::  handle_(const int_type I) noexcept :
        u( absolute(I) )
        {
        }



        integer:: handle:: handle(const integer &z) noexcept :
        handle_(),
        natural::handle(z.n),
        s(z.s)
        {
        }

        integer:: handle:: handle(const int_type &I) noexcept :
        handle_(I),
        natural::handle(u),
        s( __sign::of(I) )
        {
        }

        integer:: handle:: ~handle() noexcept
        {
            
        }


        integer:: handle:: handle(const natural &N) noexcept :
        handle_(),
        natural::handle(N),
        s( (N>0) ? positive : __zero__ )
        {
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

#include "yack/ios/decoder.hpp"

namespace yack
{
    namespace apex
    {
        integer integer:: construct(ios::istream &fp, size_t &cumul)
        {
            static const char fn[] = "integer::construct";
            int8_t       s8 = 0;
            {
                const size_t nr = ios::decoder::read(fp,s8);
                if(nr!=1) throw exception("%s(no data for sign)",fn);
                ++cumul;
            }

            const natural nv = natural::construct(fp,cumul);
            switch( __sign::of(s8) )
            {
                case negative:
                    if(nv==0) throw exception("%s(negative sign for zero)",fn);
                    return integer(negative,nv);

                case positive:
                    if(nv==0) throw exception("%s(positive sign for zero)",fn);
                    return integer(positive,nv);

                case __zero__: break;
            }
            if(nv!=0) throw exception("%s(invalid zero sign)",fn);
            return integer(0);
        }
    }

}


