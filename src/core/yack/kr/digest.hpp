#ifndef YACK_CRYPTO_DIGEST_INCLUDED
#define YACK_CRYPTO_DIGEST_INCLUDED 1

#include "yack/sequence/contiguous.hpp"

namespace yack
{

    class digest : public contiguous<uint8_t>
    {
    public:
        digest(const size_t n);
        digest(const digest &);
        virtual ~digest() throw();

        virtual size_t size() const throw();
        virtual uint8_t       & operator[](const size_t indx)       throw();
        virtual const uint8_t & operator[](const size_t indx) const throw();
        virtual uint8_t       * operator*() throw();
        virtual const uint8_t * operator*() const throw();

        friend std::ostream & operator<<(std::ostream &os, const digest &);

    private:
        YACK_DISABLE_ASSIGN(digest);
        uint8_t     *addr;
        uint8_t     *byte;
        const size_t blen;

    };

}

#endif

