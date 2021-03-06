
#include "yack/kr/digest.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include "yack/type/hexa.hpp"
#include "yack/arith/align.hpp"
#include "yack/exception.hpp"

#include <cstring>

namespace yack
{

    const char digest::clid[] = "digest";

    const char *digest:: class_uid() const throw()
    {
        return clid;
    }


    void digest:: ldz() throw()
    {
        memset(addr,0,blen);
    }

    digest::const_type * digest:: cxx() const throw()
    {
        return byte;
    }

    digest::const_type * digest:: mem() const throw()
    {
        return addr;
    }


#define YACK_DIGEST_CTOR(N) \
collection(),               \
contiguous<uint8_t>(),      \
memory::rw_buffer(),        \
ios::serializable(),        \
addr( static_cast<uint8_t*>(memory::legacy::acquire(N)) ), \
byte(addr-1),\
blen( N )

    digest:: digest(const size_t n) :
    YACK_DIGEST_CTOR(n)
    {
        
    }


    digest:: digest(const size_t n, const uint8_t b) :
    YACK_DIGEST_CTOR(n)
    {
        memset(addr,b,blen);
    }


    digest:: digest(const digest &d) :
    YACK_DIGEST_CTOR(d.blen)
    {
        memcpy(addr,d.addr,blen);
    }

    static  inline size_t check_hexa(const char *str)
    {
        if(str)
        {
            const size_t n = strlen(str);
            for(size_t i=0;i<n;++i)
            {
                const char c = str[i];
                if( hexa::convert(c) < 0 )
                {
                    throw exception("digest: invalid char '%c'",c);
                }
            }
            return YACK_ALIGN_ON(2,n) >> 1;
        }
        else
        {
            return 0;
        }
    }

    digest:: digest(const char *str) :
    YACK_DIGEST_CTOR( check_hexa(str) )
    {
        if(str)
        {
            const size_t n = strlen(str);
            const char  *p = str+n;
            for(size_t i=0;i<n;++i)
            {
                const int h = hexa::convert(*(--p)); assert(h>=0); assert(h<16);
                //std::cerr << hexa::lowercase_char[h] << std::endl;
                const size_t j = blen - (i >> 1);
                if(0==(i&1))
                {
                    byte[j] = h;
                }
                else
                {
                    byte[j] |= unsigned(h) << 4;
                }
            }
        }
    }

    digest:: digest(const memory::ro_buffer &buf) :
    YACK_DIGEST_CTOR(buf.measure())
    {
        memcpy(addr,buf.ro_addr(),blen);
    }

    digest:: digest(const void *block_addr, const size_t block_size) :
    YACK_DIGEST_CTOR(block_size)
    {
        assert(!(NULL==block_addr&&block_size>0));
        memcpy(addr,block_addr,blen);
    }


    digest:: ~digest() throw()
    {
        memset(addr,0,blen);
        memory::legacy::release(addr);
        coerce(blen) = 0;
    }

    size_t digest:: size() const throw()
    {
        return blen;
    }
    

    std::ostream & operator<<(std::ostream &os, const digest &d)
    {
        const uint8_t *p = d.addr;
        size_t         n = d.blen;
        while(n-- > 0)
        {
            os << hexa::lowercase_text[ *(p++) ];
        }
        return os;
    }

    bool operator==(const digest &lhs, const digest &rhs) throw()
    {
        size_t n = lhs.blen;
        if(rhs.blen==n)
        {
            const uint8_t *l = lhs.addr;
            const uint8_t *r = rhs.addr;
            while(n-- > 0 )
            {
                if( *(l++) != *(r++) ) return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    
    const void * digest:: ro_addr() const throw() { return addr; }
    size_t       digest:: measure() const throw() { return blen; }


}

#include "yack/ios/encoder.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    size_t digest:: serialize(ios::ostream &os) const
    {
        const size_t nw = ios::encoder::emit(os,blen);
        os.frame(addr,blen);
        return nw + blen;
    }
}

