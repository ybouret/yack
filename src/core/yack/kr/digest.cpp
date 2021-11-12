
#include "yack/kr/digest.hpp"
#include "yack/memory/allocator/legacy.hpp"
#include "yack/type/hexa.hpp"
#include <cstring>

namespace yack
{

#define YACK_DIGEST_CTOR(N) \
addr( static_cast<uint8_t*>(memory::legacy::acquire(N)) ), \
byte(addr-1),\
blen( N )

    digest:: digest(const size_t n) :
    YACK_DIGEST_CTOR(n)
    {
        
    }

    digest:: digest(const digest &d) :
    YACK_DIGEST_CTOR(d.blen)
    {
        memcpy(addr,d.addr,blen);
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

    uint8_t &  digest:: operator[](const size_t indx) throw()
    {
        assert(indx>0);
        assert(indx<=size());
        return byte[indx];
    }

    const uint8_t &  digest:: operator[](const size_t indx) const throw()
    {
        assert(indx>0);
        assert(indx<=size());
        return byte[indx];
    }

    uint8_t * digest:: operator*() throw()
    {
        return addr;
    }

    const uint8_t * digest:: operator*() const throw()
    {
        return addr;
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


}
